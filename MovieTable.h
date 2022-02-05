#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "DBHandler.h"

class MovieTable : public QObject{
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //双击 标签时发出的信号, 由mainwindow接收
	void runPlayerTrigger(QStringList paths);
public slots:
	void on_tableView_double_clicked(const QModelIndex);
	void on_movieRankUpdate(int movieid, int rank);
public:
	MovieTable(QTableView* tableview, DBHandler* dbHandler);
	void bindingModel(QSqlQueryModel* model);
	void runPlayer(QStringList qs);
private:
	QTableView* tableView;
	DBHandler* dbHandler;
};

