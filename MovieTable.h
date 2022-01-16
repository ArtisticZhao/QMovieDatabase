#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>

class MovieTable : QObject{
	Q_OBJECT

public slots:
	void on_tableView_double_clicked(const QModelIndex);
public:
	MovieTable(QTableView* tableview);
	void bindingModel(QSqlTableModel* model);
	void bindingModel(QSqlQueryModel* model);
private:
	QTableView* tableView;
};

