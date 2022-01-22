#include "FileOperator.h"
#include <QProcess>
#include <qDebug>
#include <QThread>


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
	// �������ж�ȡ��׺���б�
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


void FileOperator::runPlayer(QStringList paths) {
	for (QString path : paths)
	{
		auto qsl = QStringList(path);
		if (paths.size()>1)  // ���ֻ��һ��path����ֱ�Ӳ���
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