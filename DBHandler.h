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
	bool openDb(void);  // �����ݿ�
	bool isTableExist(QString tableName); // �ж����ݱ��Ƿ����
	int  isFieldExist(QString tableName, QString fieldName); 
	bool addFilesToDB(QList<QFileInfo> Files);
	void getAllMovies();

	SqlQueryModel* getSqlQueryModel(); //��ȡ���ݱ�
	bool setModelFilter(QList<int> selectTagId, bool isAnd);  // ɸѡtags
	
	// ��ǩ����
	void getTags(QStandardItemModel* tags, QList<int>* tagid);
	bool createTag(QString tag);
	bool editTag(int id, QString tagName);
	bool markTags(int movieid, QList<int> tagid);
	bool earseTags(int movieid, QList<int> tagid);

	// �Ǳ깦��
	bool updateRank(int movieid, int rank);
	bool updateNamePath(int movieid, QString name, QString path);  // ������name

	bool removeMovie(int movieid); // ɾ����¼
	
private:
	QSqlDatabase database;
	QSqlTableModel* model;
	SqlQueryModel* qmodel;
	bool execSql(QString oneLine);
};

