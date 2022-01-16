#include "TagFilter.h"
#include <QDebug>
void TagFilter::on_tagAdded(QString name, long id) {
	qDebug() << name << id;
	QStandardItem* Item = new QStandardItem();
	Item->setCheckable(true);
	Item->setText(name);
	tagsModel->appendRow(Item);
	tagid.append(id);
}

TagFilter::TagFilter(QListView* listView, DBHandler* dbHandler) {
	tagsModel = nullptr;
	this->listView = listView;
	this->dbHandler = dbHandler;
	// show tags
	tagsModel = new QStandardItemModel();
	dbHandler->getTags(tagsModel, &tagid);
	this->listView->setModel(tagsModel);
}
