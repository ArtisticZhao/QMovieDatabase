#include "StarDelegate.h"
#include "StarRating.h"
#include <QDebug>

void StarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	bool res;
	int rank_num = index.data().toInt(&res);
	if (res) {  // �ж��Ƿ�תΪ����
		StarRating starRating(rank_num);
		if (option.state & QStyle::State_Selected)
			painter->fillRect(option.rect, option.palette.highlight());

		starRating.paint(painter, option.rect, option.palette,
			StarRating::EditMode::ReadOnly);
	}
	else {
		QStyledItemDelegate::paint(painter, option, index);
	}
}

QSize StarDelegate::sizeHint(const QStyleOptionViewItem& option,
							const QModelIndex& index) const {
	bool res;
	int rank_num = index.data().toInt(&res);
	if (res) {  // �ж��Ƿ�תΪ����
		StarRating starRating(rank_num);
		return starRating.sizeHint();
	}

	return QStyledItemDelegate::sizeHint(option, index);
}

// �����༭���棬��������rank
QWidget* StarDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem& option,
	const QModelIndex& index) const 
{
	bool res;
	int rank_num = index.data().toInt(&res);
	if (res) {  // �ж��Ƿ�תΪ����
		StarRating starRating(rank_num);
		StarEditor* editor = new StarEditor(parent);
		connect(editor, &StarEditor::editingFinished,
			this, &StarDelegate::commitAndCloseEditor);
		return editor;
	}

	return QStyledItemDelegate::createEditor(parent, option, index);
}

// ����rank����
void StarDelegate::setEditorData(QWidget* editor,
	const QModelIndex& index) const {

	bool res;
	int rank_num = index.data().toInt(&res);
	if (res) {  // �ж��Ƿ�תΪ����
		StarRating starRating(rank_num);
		StarEditor* starEditor = qobject_cast<StarEditor*>(editor);
		starEditor->setStarRating(starRating);
		
	} else {
		QStyledItemDelegate::setEditorData(editor, index);
	}
}

// ��rank���ݸ��µ�db
void StarDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
	const QModelIndex& index) const {
	bool res;
	int rank_num = index.data().toInt(&res);
	if (res) {  // �ж��Ƿ�תΪ����
		StarEditor* starEditor = qobject_cast<StarEditor*>(editor);
		int rank = starEditor->starRating().starCount();
		model->setData(index, QVariant::fromValue(rank));
	} else {
		QStyledItemDelegate::setModelData(editor, model, index);
	}
}
void StarDelegate::commitAndCloseEditor() {
	StarEditor* editor = qobject_cast<StarEditor*>(sender());
	emit 
	(editor);
	emit closeEditor(editor);
}
