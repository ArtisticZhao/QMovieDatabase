#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
	Q_OBJECT
	
public:
	bool isSelected;
	int tagid;
	explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~ClickableLabel();

	void setCheck(bool status);

protected:
	void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H