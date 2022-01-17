#pragma once
#include <QDialog>
#include <QCloseEvent>
class TagEditDiag : public QDialog {
	Q_OBJECT
public:
	int movieid;
signals:
	void tagSelected(int movieid, QList<int> tagids);
protected:
	void closeEvent(QCloseEvent* event);
};

