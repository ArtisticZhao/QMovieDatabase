#include "DBHandler.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlRecord>

#include "MovieTableColumIndex.h"


DBHandler::DBHandler(QString path) {
	model = nullptr;
	qmodel = nullptr;
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
	updateTable();
}

void DBHandler::createTable() {
	QSqlQuery sqlQuery;
	QString query;
	query =
		"CREATE TABLE t_movies("\
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"\
		"name TEXT,"\
		"path TEXT,"\
		"rank INTEGER DEFAULT 0"\
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

bool DBHandler::updateTable() {
	if (isFieldExist("t_movies", "rank") == -1) {
		// 字段不存在
		qDebug() << "rank not exist";
		QSqlQuery query;
		if (!query.exec("ALTER TABLE t_movies ADD COLUMN rank INTEGER DEFAULT 0")) {
			qDebug() << "Error: Fail to add column 'rank'. " << query.lastError();
			return false;
		}
	}
	return true;
}

bool DBHandler::isTableExist(QString tableName) {
	QSqlDatabase database = QSqlDatabase::database();
	if (database.tables().contains(tableName)) {
		return true;
	}
	return false;
}

int DBHandler::isFieldExist(QString tableName, QString fieldName) {
	QSqlQuery query;
	QString strSql = QString("SELECT * FROM SQLITE_MASTER WHERE TYPE='table' AND NAME='%1'").arg(tableName);
	if (query.exec(strSql)) {
		if (query.next())//表存在
		{
			strSql = QString("SELECT * FROM %2").arg(tableName);
			if (query.exec(strSql)) {
				QSqlRecord record = query.record();
				int index = record.indexOf(fieldName);
				if (index == -1) {
					return -1;
				}
				else {
					return 1;
				}
			}
			else {
				qWarning() << __FUNCTION__ << "sql:" << strSql << ",lastError:" << query.lastError().text();
				return -2;
			}
		}
		else//表不存在
		{
			return 0;
		}
	}
	else {
		qWarning() << __FUNCTION__ << "sql:" << strSql << ",lastError:" << query.lastError().text();
		return -2;
	}
}

bool DBHandler::addFilesToDB(QList<QFileInfo> Files) {
	QSqlQuery sqlQuery;
	QString insert_sql = "INSERT INTO t_movies (`name`, `path`) VALUES (?, ?)";
	sqlQuery.prepare(insert_sql);
	// 批量插入记录
	QVariantList filenames;
	QVariantList paths;
	for (QFileInfo file : Files)
	{
		filenames << file.completeBaseName();
		paths << file.absoluteFilePath();
	}
	sqlQuery.addBindValue(filenames);
	sqlQuery.addBindValue(paths);

	if (!sqlQuery.execBatch()) {
		qDebug() << "Error: Fail to add files to DB. " << sqlQuery.lastError();
		return false;
	}

	QMessageBox messageBox(QMessageBox::Information,  
		QStringLiteral("扫描"), QStringLiteral("扫描完成"),
		NULL, NULL);
	messageBox.exec();
	// 在扫描完路径之后刷新tableView
	getSqlQueryModel();
	return true;
}

void DBHandler::getAllMovies() {
	//QSqlQuery query;
	//query.exec("select * from t_movies");
	// 查询所有的信息，并且把标签信息合并到一起显示
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

SqlQueryModel* DBHandler::getSqlQueryModel() {
	// 查询所有的信息，并且把标签信息合并到一起显示
	auto query = QSqlQuery(
		"SELECT t_movies.id, t_movies.name, group_concat(t_tags.name), t_movies.rank, t_movies.path "
		"FROM t_movies "
		"LEFT JOIN t_unions ON t_movies.id = t_unions.movie_id "
		"LEFT JOIN t_tags ON t_unions.tag_id = t_tags.id "
		"GROUP BY t_movies.id");
	if (qmodel == nullptr) {
		qmodel = new SqlQueryModel;
		qmodel->setQuery(query);
		qmodel->setHeaderData(static_cast<int>(MovieTableColumIndex::Name), Qt::Horizontal, QStringLiteral("电影名称"));
		qmodel->setHeaderData(static_cast<int>(MovieTableColumIndex::Tag), Qt::Horizontal, QStringLiteral("标签"));
		qmodel->setHeaderData(static_cast<int>(MovieTableColumIndex::Rank), Qt::Horizontal, QStringLiteral("星级"));
		qmodel->setHeaderData(static_cast<int>(MovieTableColumIndex::Path), Qt::Horizontal, QStringLiteral("文件路径"));
	}
	else {
		// 刷新内容
		qmodel->setQuery(query);
	}
	return qmodel;
}

void DBHandler::getTags(QStandardItemModel* model, QList<int>* tagid) {
	QSqlQuery query;
	if (!query.exec("select id, name from t_tags")) {
		qDebug() << "Error: " << query.lastError();
		return;
	}
	
	QStandardItem* Item = nullptr;
	int index = 0;
	while (query.next())
	{
		int id = query.value(0).toUInt();
		QString name = query.value(1).toString();
		qDebug() << id << name;
		Item = new QStandardItem();
		Item->setCheckable(true);
		Item->setText(name);
		model->setItem(index, Item);
		tagid->append(id);
		index++;
	}
}

bool DBHandler::createTag(QString tag) {
	QSqlQuery query;
	QString sql = QString("INSERT INTO t_tags (`name`) VALUES (\"%1\")").arg(tag);
	if (!query.exec(sql)) {
		qDebug() << "Error: " << query.lastError();
		return false;
	}
	else {
		qDebug() << query.lastInsertId();  // 获取自增ID
		emit newTagAdded(tag, query.lastInsertId().toUInt());
		return true;
	}
}

bool DBHandler::editTag(int id, QString tagName) {
	QString sql = QString("UPDATE t_tags SET name=\"%1\" WHERE id=%2;").arg(tagName).arg(id);
	QSqlQuery query;
	if (!query.exec(sql)) {
		qDebug() << "Error: " << query.lastError();
		return false;
	}
	return true;
}

// 为电影打标签
bool DBHandler::markTags(int movieid, QList<int> tagid) {
	QSqlQuery sqlQuery;
	QString insert_sql = "INSERT INTO t_unions (`movie_id`, `tag_id`) VALUES (?, ?)";
	sqlQuery.prepare(insert_sql);
	// 批量插入记录
	QVariantList movie_id;
	QVariantList tag_id;
	for (int tid : tagid) {
		movie_id << movieid;
		tag_id << tid;
	}
	sqlQuery.addBindValue(movie_id);
	sqlQuery.addBindValue(tag_id);

	if (!sqlQuery.execBatch()) {
		qDebug() << "Error: Fail to add files to DB. " << sqlQuery.lastError();
		return false;
	}
	getSqlQueryModel();
	return true;
}

// 将电影打好的标签移除
bool DBHandler::earseTags(int movieid, QList<int> tagid) {
	// DELETE FROM t_unions WHERE movie_id=1 AND tag_id=1;
	QSqlQuery sqlQuery;
	for (int tid: tagid){
		if (!sqlQuery.exec(QString("DELETE FROM t_unions WHERE movie_id=%1 AND tag_id=%2").arg(movieid).arg(tid))) {
			qDebug() << "Error: Fail to erase tag from movie. " << sqlQuery.lastError();
			return false;
		}
	}
	getSqlQueryModel();
	return true;
}

bool DBHandler::updateRank(int movieid, int rank) {
	QString sql = QString("UPDATE t_movies SET rank=%1 WHERE id=%2;").arg(rank).arg(movieid);
	QSqlQuery query;
	if (!query.exec(sql)) {
		qDebug() << "Error: " << query.lastError();
		return false;
	}
	getSqlQueryModel();
	return true;
}

bool DBHandler::updateNamePath(int movieid, QString name, QString path) {
	QString sql = QString("UPDATE t_movies SET name=\"%1\", path=\"%2\" WHERE id=%3;").arg(name).arg(path).arg(movieid);
	QSqlQuery query;
	if (!query.exec(sql)) {
		qDebug() << "Error: " << query.lastError();
		return false;
	}
	getSqlQueryModel();
	return true;
}

bool DBHandler::removeMovie(int movieid) {
	// 需要删除t_movies 和 t_union 中的全部记录
	bool res = true;
	res |= execSql(QString("DELETE FROM t_movies WHERE id=%1").arg(movieid));
	res |= execSql(QString("DELETE FROM t_unions WHERE movie_id=%1").arg(movieid));
	if (!res) return res;
	getSqlQueryModel();
	return true;
}

bool DBHandler::updatePath(int movieid, QString path) {
	QString sql = QString("UPDATE t_movies SET path=\"%1\" WHERE id=%2;").arg(path).arg(movieid);
	if (!execSql(sql)) {
		return false;
	}
	getSqlQueryModel();
	return true;
}

bool DBHandler::execSql(QString oneLine) {
	QSqlQuery query;
	if (!query.exec(oneLine)) {
		qDebug() << "Error: " << query.lastError();
		return false;
	}
	return true;
}

bool DBHandler::setModelFilter(QList<int> selectTagId, bool isAnd) {
	if (qmodel == nullptr) return false;
	if (selectTagId.size()==0) {
		// 筛选标签为空, 重置筛选器
		getSqlQueryModel();
		return true;
	}
	if (!isAnd) {
		// OR 查询只要包含的标签
		QString sub_query = QString("%1").arg(selectTagId.at(0));
		for (int i = 1; i < selectTagId.size(); i++) {
			sub_query += QString(",%1").arg(selectTagId.at(i));
		}
		auto query = QSqlQuery(
			QString("SELECT t_movies.id, t_movies.name, group_concat(t_tags.name), t_movies.path "
				"FROM t_movies "
				"LEFT JOIN t_unions ON t_movies.id = t_unions.movie_id "
				"LEFT JOIN t_tags ON t_unions.tag_id = t_tags.id "
				"WHERE t_movies.id IN ("
				"SELECT movie_id FROM t_unions WHERE tag_id IN (%1)"
				")"
				"GROUP BY t_movies.id").arg(sub_query));
		qmodel->setQuery(query);
	}
	else {
		// AND
		QString sub_query = QString("SELECT movie_id FROM t_unions WHERE tag_id = %1").arg(selectTagId.at(0));
		for (int i = 1; i < selectTagId.size(); i++) {
			sub_query += QString(" INTERSECT SELECT movie_id FROM t_unions WHERE tag_id = %1").arg(selectTagId.at(i));
		}
		auto query = QSqlQuery(
			QString("SELECT t_movies.id, t_movies.name, group_concat(t_tags.name), t_movies.path "
				"FROM t_movies "
				"LEFT JOIN t_unions ON t_movies.id = t_unions.movie_id "
				"LEFT JOIN t_tags ON t_unions.tag_id = t_tags.id "
				"WHERE t_movies.id IN ("
				"%1"
				")"
				"GROUP BY t_movies.id").arg(sub_query));
		qmodel->setQuery(query);
	}
	return true;
	
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
