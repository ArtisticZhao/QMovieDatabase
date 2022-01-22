#pragma once
#include <QDialog>
#include <QCloseEvent>
#include <QStandardItemModel>

class TagEditDiag : public QDialog {
	Q_OBJECT
public:
	int movieid;
	TagEditDiag(QWidget* parent, int movid, QStringList tags, QList<int>& tagid, QStandardItemModel* tagsModel);
private:
	QSet<int> oldSelectedID;
signals:
	void tagPreselected(int movieid, QList<int> tagids);
	void tagPreremoved(int movieid, QList<int> tagids);
protected:
	void closeEvent(QCloseEvent* event);
};

