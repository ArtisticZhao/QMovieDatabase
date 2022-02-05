#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "DBHandler.h"

class MovieTable : public QObject{
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //˫�� ��ǩʱ�������ź�, ��mainwindow����
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

