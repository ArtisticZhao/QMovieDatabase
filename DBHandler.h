#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DBHandler {
public:
	DBHandler(QString path);
	void createTable();
	// �����ݿ�
	bool openDb(void);
	// �ж����ݱ��Ƿ����
	bool isTableExist(QString& tableName);
private:
	QSqlDatabase database;
	
};

