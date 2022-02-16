#include "MovieTable.h"
#include "StarDelegate.h"
#include "MovieTableColumIndex.h"
#include <QDebug>


MovieTable::MovieTable(MovieTableView* tableview, DBHandler* dbHandler) {
	this->tableView = tableview;
	this->dbHandler = dbHandler;
	connect(tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_tableView_double_clicked(const QModelIndex)));
}


void MovieTable::on_tableView_double_clicked(const QModelIndex itemIndex) {
	if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Tag)) {
		// ˫����ǩ
		auto tagstr = tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column())).toString();
		QStringList tags = tagstr.split(",");
		emit tagEditTrigger(tableView->model()->data(tableView->model()->index(itemIndex.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt(), tags);
	}
	else if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Rank))
	{
		// ˫��rank
		auto item = tableView->model()->flags(itemIndex);
		tableView->edit(itemIndex);
	}
	else {
		qDebug() << tableView->model()->data(tableView->model()->index(itemIndex.row(), itemIndex.column()));  // ��ǰ˫��
		// ����������
		emit runPlayerTrigger(QStringList(tableView->model()->data(tableView->model()->index(itemIndex.row(), static_cast<int>(MovieTableColumIndex::Path))).toString()));
	}
	
}


void MovieTable::on_movieRankUpdate(int movieid, int rank) {
	// db
	dbHandler->updateRank(movieid, rank);
}



void MovieTable::on_movieNamePathUpdate(int movieid, QString name, QString path) {
	dbHandler->updateNamePath(movieid, name, path);
}

void MovieTable::bindingModel(QSqlQueryModel* model) {
	this->tableView->setModel(model);
	// ����id��
	this->tableView->setColumnHidden(0, true);
	// �����Ǽ���ʾ
	this->tableView->setItemDelegateForColumn(3, new StarDelegate);
	// �󶨸����ź�
	connect(model, SIGNAL(movieRankUpdate(int, int)), this, SLOT(on_movieRankUpdate(int, int)));
	connect(model, SIGNAL(movieNamePathUpdate(int, QString, QString)), this, SLOT(on_movieNamePathUpdate(int, QString, QString)));
}


void MovieTable::runPlayer(QStringList qs) {
	// ����������
	emit runPlayerTrigger(qs);
}


