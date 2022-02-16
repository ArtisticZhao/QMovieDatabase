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

public:
	FileOperator(Settings* settings);
	QList<QFileInfo> pathWalk(QString dirpath);
	static bool isLegalFileName(QString filename);
	static QString renameFile(QString fullpath, QString filename);

public slots:
	void runPlayer(QStringList paths);
};

