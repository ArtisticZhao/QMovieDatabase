#include "TagFilter.h"
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include "ClickableLabel.h"

void TagFilter::on_tagAdded(QString name, long id) {
	qDebug() << name << id;
	QStandardItem* Item = new QStandardItem();
	Item->setCheckable(true);
	Item->setText(name);
	tagsModel->appendRow(Item);
	tagid.append(id);
}

void TagFilter::on_tagClicked() {
	qDebug() << "on clicked in tag";
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

void TagFilter::editTags(QWidget* parent) {
#define COLS 4
	QDialog* qd = new QDialog(parent);
	QList<QLabel*> tagLabels;
	QGridLayout* gridLayout = new QGridLayout();
	for (int i=0; i<tagid.size(); i++)
	{
		ClickableLabel* label = new ClickableLabel(qd);
		label->setText(tagsModel->item(i)->text());
		label->adjustSize();
		
		gridLayout->addWidget(label, i / COLS, i % COLS);
		connect(label, SIGNAL(clicked()), this, SLOT(on_tagClicked()));
	}
	qd->setLayout(gridLayout);
	qd->show();
}
