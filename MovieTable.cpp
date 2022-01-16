#include "MovieTable.h"

MovieTable::MovieTable(QTableView* tableview) {
	this->tableview = tableview;
}

void MovieTable::bindingModel(QSqlTableModel* model) {
	this->tableview->setModel(model);
}

void MovieTable::bindingModel(QSqlQueryModel* model) {
	this->tableview->setModel(model);
}

