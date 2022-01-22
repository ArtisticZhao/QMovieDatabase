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

void TagFilter::on_tagPreselected(int movieid, QList<int> tagids) {
	// save to db;
	dbHandler->markTags(movieid, tagids);
}

void TagFilter::on_tagPreremoved(int movieid, QList<int> tagids) {
	dbHandler->earseTags(movieid, tagids);
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

void TagFilter::on_ItemChanged(QStandardItem* item) {
	auto index = tagsModel->indexFromItem(item).row();
	if (tagid.size() < index) return;

	int tagChangedID = tagid.at(index);

	dbHandler->editTag(tagChangedID, item->text());
	// ������ֺ�ˢ��
	dbHandler->setModelFilter(selectedTagId, rb_selectModeAnd->isChecked());
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
	// tagFilter �����¼�, �ж��Ƿ����ɸѡ
	connect(listView, SIGNAL(clicked(const QModelIndex)), this, SLOT(on_listView_clicked(const QModelIndex)));
	//
	connect(tagsModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(on_ItemChanged(QStandardItem *)));
}

// �����༭ӰƬtags����
void TagFilter::editTags(QWidget* parent, int movid, QStringList tags) {
	qd = new TagEditDiag(parent, movid, tags, tagid, tagsModel);
	connect(qd, SIGNAL(tagPreselected(int, QList<int>)), this, SLOT(on_tagPreselected(int, QList<int>)));
	connect(qd, SIGNAL(tagPreremoved(int, QList<int>)), this, SLOT(on_tagPreremoved(int, QList<int>)));
}
