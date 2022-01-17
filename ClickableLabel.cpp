#include "clickablelabel.h"
#include <QDebug>
ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent) {
	isSelected = false;
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
	isSelected = !isSelected;
	if (isSelected) {
		this->setStyleSheet("QLabel{border:2px solid rgb(0, 105, 246);}"); // ���ñ߿��ʾѡ��
	}
	else {
		this->setStyleSheet(""); // ����߿�
	}
	emit clicked();
}
