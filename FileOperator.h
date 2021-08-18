#pragma once
#include <QString.h>
#include <qstringlist.h>
#include <qdir.h>
#include <QDirIterator>

class FileOperator
{
public:
	static QList<QFileInfo> pathWalk(QString dirpath);
};

