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
	// �����ݿ�
	bool openDb(void);
	// �ж����ݱ��Ƿ����
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

