#pragma once
#include <QStyledItemDelegate>
#include "StarEditor.h"

class StarDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	using QStyledItemDelegate::QStyledItemDelegate;

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const override;

private slots:
	void commitAndCloseEditor();
}
;