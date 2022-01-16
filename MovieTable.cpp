#include "MovieTable.h"
#include <QDebug>

MovieTable::MovieTable(QTableView* tableview) {
	this->tableView = tableview;
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_tableView_double_clicked(const QModelIndex)));
}

void MovieTable::on_tableView_double_clicked(const QModelIndex itemIndex) {
	qDebug() << itemIndex.row() << itemIndex.column();
	qDebug() << tableView->model()->data( tableView->model()->index(itemIndex.row(), itemIndex.column()));
	qDebug() << "movie id is " << tableView->model()->data(tableView->model()->index(itemIndex.row(), 0));
}

void MovieTable::bindingModel(QSqlTableModel* model) {
	this->tableView->setModel(model);
}

void MovieTable::bindingModel(QSqlQueryModel* model) {
	this->tableView->setModel(model);
	// вўВиidСа
	this->tableView->setColumnHidden(0, true);
}

