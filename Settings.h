#pragma once
#include <QString>

class Settings
{
public:
	QString dbPath;
	QString playerPath;
	Settings();
private:
	void openOrCreate();
};

