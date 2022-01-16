#include "DBHandler.h"
#include <QDebug>

DBHandler::DBHandler(QString path) {
	model = nullptr;
	qmodel = nullptr;
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
	// ִ��sql���
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

bool DBHandler::addFilesToDB(QList<QFileInfo> Files) {
	QSqlQuery sqlQuery;
	QString insert_sql = "INSERT INTO t_movies (`name`, `path`) VALUES (?, ?)";
	sqlQuery.prepare(insert_sql);
	// ���������¼
	QVariantList filenames;
	QVariantList paths;
	for (QFileInfo file : Files)
	{
		qDebug() << file.baseName() << file.absoluteFilePath();
		filenames << file.baseName();
		paths << file.absoluteFilePath();
	}
	sqlQuery.addBindValue(filenames);
	sqlQuery.addBindValue(paths);

	if (!sqlQuery.execBatch()) {
		qDebug() << "Error: Fail to add files to DB. " << sqlQuery.lastError();
		return false;
	}
	qDebug() << "add to DB";
	getAllMovies();
	return true;
}

void DBHandler::getAllMovies() {
	//QSqlQuery query;
	//query.exec("select * from t_movies");
	// ��ѯ���е���Ϣ�����Ұѱ�ǩ��Ϣ�ϲ���һ����ʾ
	auto querys = QString(
		"SELECT t_movies.*, group_concat(t_tags.name) "
		"FROM t_movies "
		"LEFT JOIN t_unions ON t_movies.id = t_unions.movie_id "
		"LEFT JOIN t_tags ON t_unions.tag_id = t_tags.id "
		"GROUP BY t_movies.id");
	QSqlQuery query;
	if (!query.exec(querys)) {
		qDebug() << "Error: " << query.lastError();
		return;
	}
	
	while (query.next()) {
		QString name = query.value(1).toString();
		QString path = query.value(2).toString();
		QString tags = query.value(3).toString();
		qDebug() << name << path << tags;
	}
}

QSqlTableModel* DBHandler::getSqlTableModel() {
	if (model == nullptr) {
		model = new QSqlTableModel(nullptr, database);
		//model->setTable("t_movies");
		//model->setEditStrategy(QSqlTableModel::OnManualSubmit);
		//model->select();
		//model->removeColumn(0); // don't show the ID
		//model->setHeaderData(0, Qt::Horizontal, QString("Column 1"));
		//model->setHeaderData(1, Qt::Horizontal, QString("Column 2"));
	}
	return model;
}

QSqlQueryModel* DBHandler::getSqlQueryModel() {
	if (qmodel == nullptr) {
		qmodel = new QSqlQueryModel;
		// ��ѯ���е���Ϣ�����Ұѱ�ǩ��Ϣ�ϲ���һ����ʾ
		auto query = QSqlQuery(
			"SELECT t_movies.name, group_concat(t_tags.name), t_movies.path "
			"FROM t_movies "
			"LEFT JOIN t_unions ON t_movies.id = t_unions.movie_id "
			"LEFT JOIN t_tags ON t_unions.tag_id = t_tags.id "
			"GROUP BY t_movies.id");
		qmodel->setQuery(query);
		qmodel->setHeaderData(0, Qt::Horizontal, QStringLiteral("��Ӱ����"));
		qmodel->setHeaderData(1, Qt::Horizontal, QStringLiteral("��ǩ"));
		qmodel->setHeaderData(2, Qt::Horizontal, QStringLiteral("�ļ�·��"));
	}
	return qmodel;
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
