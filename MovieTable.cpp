#include "MovieTable.h"
#include <QDebug>
#include "StarDelegate.h"

MovieTable::MovieTable(QTableView* tableview, DBHandler* dbHandler) {
	this->tableView = tableview;
	this->dbHandler = dbHandler;
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_tableView_double_clicked(const QModelIndex)));
}


void MovieTable::on_tableView_double_clicked(const QModelIndex itemIndex) {
	if (itemIndex.column() == 2) {
		// ˫����ǩ
		auto tagstr = tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column())).toString();
		QStringList tags = tagstr.split(",");
		emit tagEditTrigger(tableView->model()->data(tableView->model()->index(itemIndex.row(), 0)).toInt(), tags);
	}
	else if (itemIndex.column() == 3)
	{
		// ˫��rank
		auto item = tableView->model()->flags(itemIndex);
		
		tableView->edit(itemIndex);
	}
	else {
		//qDebug() << tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column()));  // ��ǰ˫��
		// ����������
		emit runPlayerTrigger(QStringList(tableView->model()->data(tableView->model()->index(itemIndex.row(), 3)).toString()));
	}
	
}


void MovieTable::on_movieRankUpdate(int movieid, int rank) {
	// db
	dbHandler->updateRank(movieid, rank);
}

void MovieTable::bindingModel(QSqlQueryModel* model) {
	this->tableView->setModel(model);
	// ����id��
	this->tableView->setColumnHidden(0, true);
	// �����Ǽ���ʾ
	this->tableView->setItemDelegateForColumn(3, new StarDelegate);
	// ���Ǽ������ź�
	connect(model, SIGNAL(movieRankUpdate(int, int)), this, SLOT(on_movieRankUpdate(int, int)));
}


void MovieTable::runPlayer(QStringList qs) {
	// ����������
	emit runPlayerTrigger(qs);
}

