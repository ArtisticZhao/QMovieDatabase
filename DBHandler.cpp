#include "DBHandler.h"
#include <QDebug>

DBHandler::DBHandler(QString path) {
	if (QSqlDatabase::contains("qt_sql_default_connection")) {
		database = QSqlDatabase::database("qt_sql_default_connection");
	}
	else {
		// ������SQlite���ݿ������
		database = QSqlDatabase::addDatabase("QSQLITE");
		// �������ݿ��ļ�������
		database.setDatabaseName(path);
	}
	openDb();
	if (!isTableExist(QString("t_movie")))
	{
		createTable();
	}
}

void DBHandler::createTable() {
	QSqlQuery sqlQuery;
	sqlQuery.prepare("CREATE TABLE t_movie (ID	TEXT PRIMARY KEY, Name TEXT, Path TEXT)");
	// ִ��sql���
	if (!sqlQuery.exec()) {
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
