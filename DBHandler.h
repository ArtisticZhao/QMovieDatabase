#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <qdir.h>

class DBHandler : public QObject{
	Q_OBJECT
signals:
	void newTagAdded(QString tag, long id);
public:
	DBHandler(QString path);
	void createTable();
	// �����ݿ�
	bool openDb(void);
	// �ж����ݱ��Ƿ����
	bool isTableExist(QString tableName);
	bool addFilesToDB(QList<QFileInfo> Files);
	void getAllMovies();
	QSqlQueryModel* getSqlQueryModel();
	void getTags(QStandardItemModel* tags, QList<int>* tagid);
	bool createTag(QString tag);
	bool editTag(int id, QString tagName);
	bool markTags(int movieid, QList<int> tagid);
	bool setModelFilter(QList<int> selectTagId, bool isAnd);
private:
	QSqlDatabase database;
	QSqlTableModel* model;
	QSqlQueryModel* qmodel;
};

