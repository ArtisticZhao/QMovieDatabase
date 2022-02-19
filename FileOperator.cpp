#include "FileOperator.h"
#include <QFileDialog>
#include <QProcess>
#include <qDebug>
#include <QThread>

Settings FileOperator::settings_static = Settings();

FileOperator::FileOperator(Settings* settings) {
	this->settings = settings;
}

//************************************
// Method:      pathWalk
// FullName:    FileOperator::pathWalk
// Description: Get all files in dirpath, return QList<QFileInfo> 
//				is easy to filter.
// Access:      public static 
// Returns:     QT_NAMESPACE::QList<QT_NAMESPACE::QFileInfo>
// Parameter:   QString dirpath
//************************************
QList<QFileInfo> FileOperator::pathWalk(QString dirpath) {
	// 从设置中读取后缀名列表
	QStringList nameFilters;
	QString filter = settings->fileFilter;
	QStringList filters = filter.split(' ');
	for (QString ft : filters)
	{
		nameFilters << QString("*.%1").arg(ft);
	}
	QDirIterator iter(dirpath, nameFilters,
		QDir::Files | QDir::NoSymLinks,
		QDirIterator::Subdirectories);

	QList<QFileInfo> files;
	while (iter.hasNext()) {
		iter.next();
		files << iter.fileInfo();
	}

	return files;
}


bool FileOperator::isLegalFileName(QString filename) {
	// 检查文件名是否合法，仅Windows下
	if (filename.isEmpty()) {
		return false;
	}
	bool is_legal = true;
	QString pattern("[\\\\/:*?\"<>]");
	QRegExp rx(pattern);
	int match = filename.indexOf(rx);
	if (match >= 0) {
		is_legal = false;
	}
	return is_legal;
}

QString FileOperator::renameFile(QString fullpath, QString filename) {
	QFile file(fullpath);
	QFileInfo fileInfo(file.fileName());
	QFileInfo newfi(fileInfo.absolutePath() + "/" + filename + "." + fileInfo.suffix());
	file.rename(fullpath, newfi.absoluteFilePath());
	return newfi.absoluteFilePath();
}

QString FileOperator::getAbsDir(QString filepath) {
	QFileInfo fileInfo(filepath);
	return fileInfo.absolutePath();
}

bool FileOperator::isExistFile(QString fullpath) {
	QFileInfo fileInfo(fullpath);
	return fileInfo.isFile();
}

bool FileOperator::deleteFile(QString fullpath) {
	QFile file(fullpath);
	return file.remove();
}

QString FileOperator::selectFile(QString dir) {
	QFileDialog fd;
	fd.setWindowTitle(QStringLiteral("选择替换文件"));
	fd.setDirectory(dir);  // 设置默认文件夹为旧文件所在的文件夹  FileOperator::getAbsDir(oldpath)
	fd.setFileMode(QFileDialog::ExistingFile); // 单选文件
	// 设置选择文件过滤器
	QString filter = settings_static.fileFilter;
	QStringList filters = filter.split(' ');
	QString nameFilters;
	for (QString ft : filters) {
		nameFilters += QString(" *.%1").arg(ft);
	}
	fd.setNameFilter(nameFilters);
	fd.setViewMode(QFileDialog::Detail); //设置视图模式
	if (fd.exec()) {
		auto files = fd.selectedFiles();
		if (files.length() > 0) {
			return files.at(0);
		}
	}
	return "";
}

void FileOperator::runPlayer(QStringList paths) {
	for (QString path : paths)
	{
		auto qsl = QStringList(path);
		if (paths.size()>1)  // 如果只有一条path，就直接播放
		{
			qsl << "/add";
		}
		else {
			qsl << "/current";
		}
		QProcess::startDetached(settings->playerPath, qsl);
		QThread::msleep(50);
	}
}