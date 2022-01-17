#include "TagEditDiag.h"
#include <QDebug>
#include <QLayout>
#include "ClickableLabel.h"

void TagEditDiag::closeEvent(QCloseEvent* event) {
	qDebug() << "on close";
	QList<int> tagid;
	for (int i=0; i<this->layout()->count(); i++)
	{
		ClickableLabel* label = qobject_cast<ClickableLabel*>(this->layout()->itemAt(i)->widget());
		if (label->isSelected) {
			tagid.append(label->tagid);
		}
	}
	emit tagSelected(movieid, tagid);
	this->disconnect();
	event->accept();
}
