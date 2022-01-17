#pragma once
#include <QString.h>
#include <qstringlist.h>
#include <qdir.h>
#include <QDirIterator>

class FileOperator : public QObject{
	Q_OBJECT

public:
	static QList<QFileInfo> pathWalk(QString dirpath);

public slots:
	void runPlayer(QStringList paths);
};

