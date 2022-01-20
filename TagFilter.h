#pragma once
#include <QListView>
#include <QStandardItemModel>
#include <QRadioButton>

#include "TagEditDiag.h"
#include "DBHandler.h"

class TagFilter : public QObject {
	Q_OBJECT
public slots:
	void on_tagAdded(QString name, long id);
	void on_tagSelected(int movieid, QList<int> tagids);
	void on_listView_clicked(const QModelIndex& index);
	void on_ItemChanged(QStandardItem* item);

private:
	QListView* listView;
	DBHandler* dbHandler;
	QRadioButton* rb_selectModeAnd;
	QList<int> tagid;
	QStandardItemModel* tagsModel;
	TagEditDiag* qd;
	QList<int> selectedTagId;
public:
	TagFilter(QListView*, DBHandler*, QRadioButton*);
	void editTags(QWidget* parent, int movid, QStringList tags);

};

