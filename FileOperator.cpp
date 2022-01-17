#include "FileOperator.h"
#include <QProcess>


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
	QStringList nameFilters;
	nameFilters << "*.*";
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
	paths.append(" /add");
	QProcess::startDetached("C:\\Program Files\\DAUM\\PotPlayer\\PotPlayerMini64.exe", paths);
}