#pragma once
#include <QString.h>
#include <qstringlist.h>
#include <qdir.h>
#include <QDirIterator>

#include "Settings.h"

class FileOperator : public QObject{
	Q_OBJECT
private:
	Settings* settings;

	static Settings settings_static;
public:
	FileOperator(Settings* settings);
	QList<QFileInfo> pathWalk(QString dirpath);
	static bool isLegalFileName(QString filename);
	static QString renameFile(QString fullpath, QString filename);
	static QString getAbsDir(QString filepath);
	static bool isExistFile(QString fullpath);
	static bool deleteFile(QString fullpath);

	static QString selectFile(QString dir);
	static void openInFolder(QString& fullpath);

public slots:
	void runPlayer(QStringList paths);
};

