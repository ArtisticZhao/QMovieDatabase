#include "DBHandler.h"
#include <QDebug>

DBHandler::DBHandler(QString path) {
	if (QSqlDatabase::contains("qt_sql_default_connection")) {
		database = QSqlDatabase::database("qt_sql_default_connection");
	}
	else {
		// 建立和SQlite数据库的连接
		database = QSqlDatabase::addDatabase("QSQLITE");
		// 设置数据库文件的名字
		database.setDatabaseName(path);
	}
	openDb();
	if (!isTableExist(QString("t_movies")))
	{
		createTable();
	}
}

void DBHandler::createTable() {
	QSqlQuery sqlQuery;
	QString query;
	query =
		"CREATE TABLE t_movies("\
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"\
		"name TEXT,"\
		"path TEXT"\
		");"
	;
	if (!sqlQuery.exec(query)) {
		qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
		return;
	}
	query = 
		"CREATE TABLE t_tags ("\
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"\
		"name VARCHAR(30)"\
		");"
	;
	if (!sqlQuery.exec(query)) {
		qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
		return;
	}
	query = 
		"CREATE TABLE t_unions ("\
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"\
		"movie_id INTEGER NULL,"\
		"tag_id INTEGER NULL,"\
		"CONSTRAINT fk_movie_id FOREIGN KEY (movie_id) REFERENCES t_movies (ID),"\
		"CONSTRAINT fk_tag_id FOREIGN KEY(tag_id) REFERENCES t_tags(id)"\
		");"
	;
	// 执行sql语句
	if (!sqlQuery.exec(query)) {
		qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
	}
	else {
		qDebug() << "Table created!";
	}
}

bool DBHandler::isTableExist(QString& tableName) {
	QSqlDatabase database = QSqlDatabase::database();
	if (database.tables().contains(tableName)) {
		return true;
	}

	return false;
}

bool DBHandler::openDb(void) {
	if (!database.open()) {
		qDebug() << "Error: Failed to connect database." << database.lastError();
	}
	else {
		// do something
	}

	return true;
}
