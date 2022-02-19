#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <qdir.h>

#include "SqlQueryModel.h"


class DBHandler : public QObject{
	Q_OBJECT
signals:
	void newTagAdded(QString tag, long id);
public:
	DBHandler(QString path);
	void createTable();
	bool updateTable();
	bool openDb(void);  // 打开数据库
	bool isTableExist(QString tableName); // 判断数据表是否存在
	int  isFieldExist(QString tableName, QString fieldName); 
	bool addFilesToDB(QList<QFileInfo> Files);
	void getAllMovies();

	SqlQueryModel* getSqlQueryModel(); //获取数据表
	bool setModelFilter(QList<int> selectTagId, bool isAnd);  // 筛选tags
	
	// 标签功能
	void getTags(QStandardItemModel* tags, QList<int>* tagid);
	bool createTag(QString tag);
	bool editTag(int id, QString tagName);
	bool markTags(int movieid, QList<int> tagid);
	bool earseTags(int movieid, QList<int> tagid);

	// 星标功能
	bool updateRank(int movieid, int rank);
	bool updateNamePath(int movieid, QString name, QString path);  // 重命名name

	bool removeMovie(int movieid); // 删除记录
	
private:
	QSqlDatabase database;
	QSqlTableModel* model;
	SqlQueryModel* qmodel;
	bool execSql(QString oneLine);
};

