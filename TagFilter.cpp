#include "TagFilter.h"
#include <QDebug>

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

void TagFilter::on_tagSelected(int movieid, QList<int> tagids) {
	qDebug() << movieid << tagids;
	// save to db;
	dbHandler->markTags(movieid, tagids);
}

void TagFilter::on_listView_clicked(const QModelIndex& index) {
	int id_index = tagid.at(index.row());
	if (tagsModel->item(index.row(), 0)->checkState()) {
		// ��ѡ����Ŀ
		if (std::find(selectedTagId.begin(), selectedTagId.end(), id_index) == selectedTagId.end()) {
			// û��ѡ��
			selectedTagId.append(id_index);
			// ����ɸѡ
			dbHandler->setModelFilter(selectedTagId, rb_selectModeAnd->isChecked());
		}
	}
	else {
		if (selectedTagId.removeOne(id_index)) {
			// �Ƴ��ɹ���˵������ѡ���������ǩ��������Ҫ����ɸѡ��
			dbHandler->setModelFilter(selectedTagId, rb_selectModeAnd->isChecked());
		}
	}
	
}

TagFilter::TagFilter(QListView* listView, DBHandler* dbHandler, QRadioButton* rb) {
	tagsModel = nullptr;
	qd = nullptr;
	this->listView = listView;
	this->dbHandler = dbHandler;
	this->rb_selectModeAnd = rb;
	// show tags
	tagsModel = new QStandardItemModel();
	dbHandler->getTags(tagsModel, &tagid);
	this->listView->setModel(tagsModel);
	
	connect(listView, SIGNAL(clicked(const QModelIndex)), this, SLOT(on_listView_clicked(const QModelIndex)));
}

void TagFilter::editTags(QWidget* parent, int movid, QStringList tags) {
#define COLS 4
	qd = new TagEditDiag();
	qd->setParent(parent);
	qd->movieid = movid;
	qd->setAttribute(Qt::WA_DeleteOnClose);
	QList<QLabel*> tagLabels;
	QGridLayout* gridLayout = new QGridLayout();
	for (int i=0; i<tagid.size(); i++)
	{
		ClickableLabel* label = new ClickableLabel(qd);
		label->setText(tagsModel->item(i)->text());
		label->tagid = tagid.at(i);
		label->adjustSize();
		
		gridLayout->addWidget(label, i / COLS, i % COLS);
	}
	qd->setLayout(gridLayout);
	// ��ֹ�Ӵ��ڹر�ǰ����������
	Qt::WindowFlags flags = Qt::Dialog;
	qd->setWindowFlags(flags);
	qd->setWindowModality(Qt::ApplicationModal);
	connect(qd, SIGNAL(tagSelected(int, QList<int>)), this, SLOT(on_tagSelected(int, QList<int>)));
	qd->show();
}
