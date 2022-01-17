#pragma once
#include <QString>

class Settings
{
public:
	QString dbPath;
	QString playerPath;
	QString fileFilter;
	Settings();
private:
	void openOrCreate();
};

