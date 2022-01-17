#include "MovieTable.h"
#include <QDebug>


MovieTable::MovieTable(QTableView* tableview) {
	this->tableView = tableview;
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_tableView_double_clicked(const QModelIndex)));
}

void MovieTable::on_tableView_double_clicked(const QModelIndex itemIndex) {
	qDebug() << itemIndex.row() << itemIndex.column();
	if (itemIndex.column() == 2) {
		// Ë«»÷±êÇ©
		auto tagstr = tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column())).toString();
		QStringList tags = tagstr.split(",");
		emit tagEditTrigger(tableView->model()->data(tableView->model()->index(itemIndex.row(), 0)).toInt(), tags);
	}
	else {
		//qDebug() << tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column()));  // µ±Ç°Ë«»÷
		// Æô¶¯²¥·ÅÆ÷
		emit runPlayerTrigger(QStringList(tableView->model()->data(tableView->model()->index(itemIndex.row(), 3)).toString()));
	}
	
}

void MovieTable::bindingModel(QSqlTableModel* model) {
	this->tableView->setModel(model);
}

void MovieTable::bindingModel(QSqlQueryModel* model) {
	this->tableView->setModel(model);
	// Òþ²ØidÁÐ
	this->tableView->setColumnHidden(0, true);
}

