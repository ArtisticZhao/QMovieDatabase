#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "DBHandler.h"
#include "MovieTableView.h"

class MovieTable : public QWidget{
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //˫�� ��ǩʱ�������ź�, ��mainwindow����
	void runPlayerTrigger(QStringList paths);
public slots:
	void on_tableView_double_clicked(const QModelIndex);
	void on_movieRankUpdate(int movieid, int rank);
	void on_movieNamePathUpdate(int movieid, QString name, QString path);
public:
	MovieTable(MovieTableView* tableview, DBHandler* dbHandler);
	void bindingModel(QSqlQueryModel* model);
	void runPlayer(QStringList qs);

private:
	MovieTableView* tableView;
	DBHandler* dbHandler;
};

