#include "TagEditDiag.h"
#include <QDebug>
#include <QLayout>

#include "ClickableLabel.h"

TagEditDiag::TagEditDiag(QWidget* parent, int movid, QStringList tags, QList<int>& tagid, QStandardItemModel* tagsModel)
	:QDialog(parent){
#define COLS 4
	movieid = movid;
	this->setAttribute(Qt::WA_DeleteOnClose);  // 设置删除变量的方式
	QList<QLabel*> tagLabels;
	QGridLayout* gridLayout = new QGridLayout();
	for (int i = 0; i < tagid.size(); i++) {
		ClickableLabel* label = new ClickableLabel(this);
		label->setText(tagsModel->item(i)->text());
		label->tagid = tagid.at(i);
		// 如果已经有包含的，则高亮显示
		if (tags.contains(tagsModel->item(i)->text())) {
			label->setCheck(true);
			oldSelectedID.insert(tagid.at(i));   // 保存变更前有哪些已经存在的tagID
			qDebug() << "inserting " << tagid.at(i);
		}
		label->adjustSize();
		gridLayout->addWidget(label, i / COLS, i % COLS);
	}
	qDebug() << oldSelectedID;
	this->setLayout(gridLayout);
	// 禁止子窗口关闭前操作主窗口
	Qt::WindowFlags flags = Qt::Dialog;
	this->setWindowFlags(flags);
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
}

void TagEditDiag::closeEvent(QCloseEvent* event) {
	QList<int> tagid;
	for (int i=0; i<this->layout()->count(); i++)
	{
		ClickableLabel* label = qobject_cast<ClickableLabel*>(this->layout()->itemAt(i)->widget());
		if (label->isSelected) {
			tagid.append(label->tagid);
		}
	}
	auto tagid_set = tagid.toSet();
	QSet<int> saveOld = oldSelectedID;
	QSet<int> intersection = oldSelectedID.intersect(tagid_set);  // 求原有集合和新选择后的集合的交集, 求解之后oldSelectedID会改变
	QSet<int> deleteTagID = saveOld - intersection;
	QSet<int> newTagID = tagid_set - intersection;
	emit tagPreselected(movieid, newTagID.toList());
	emit tagPreremoved(movieid, deleteTagID.toList());
	this->disconnect();
	event->accept();
}
