#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
class MovieTable {
public:
	MovieTable(QTableView* tableview);
	void bindingModel(QSqlTableModel* model);
	void bindingModel(QSqlQueryModel* model);
private:
	QTableView* tableview;
};

