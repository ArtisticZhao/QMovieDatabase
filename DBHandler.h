#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DBHandler {
public:
	DBHandler(QString path);
	void createTable();
	// 打开数据库
	bool openDb(void);
	// 判断数据表是否存在
	bool isTableExist(QString& tableName);
private:
	QSqlDatabase database;
	
};

