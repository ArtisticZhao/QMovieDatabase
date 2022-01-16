#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <qdir.h>

class DBHandler {
public:
	DBHandler(QString path);
	void createTable();
	// 打开数据库
	bool openDb(void);
	// 判断数据表是否存在
	bool isTableExist(QString& tableName);
	bool addFilesToDB(QList<QFileInfo> Files);
	void getAllMovies();
	QSqlTableModel* getSqlTableModel();
	QSqlQueryModel* getSqlQueryModel();
private:
	QSqlDatabase database;
	QSqlTableModel* model;
	QSqlQueryModel* qmodel;
};

