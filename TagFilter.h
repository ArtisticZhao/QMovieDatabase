#pragma once
#include <QListView>
#include "DBHandler.h"
#include <QStandardItemModel>

class TagFilter : public QObject {
	Q_OBJECT
public slots:
	void on_tagAdded(QString name, long id);
	void on_tagClicked();
private:
	QListView* listView;
	DBHandler* dbHandler;
	QList<int> tagid;
	QStandardItemModel* tagsModel;
public:
	TagFilter(QListView*, DBHandler*);
	void editTags(QWidget* parent);

};

