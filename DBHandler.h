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
	// 打开数据库
	bool openDb(void);
	// 判断数据表是否存在
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

