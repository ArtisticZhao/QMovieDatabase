#include "clickablelabel.h"
#include <QDebug>
#include <QLayout>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
	: QLabel(parent) {
	isSelected = false;
}

ClickableLabel::~ClickableLabel() {
}

void ClickableLabel::setCheck(bool status) {
	isSelected = status;
	if (isSelected) {
		this->setStyleSheet("QLabel{border:2px solid rgb(0, 105, 246);}"); // ���ñ߿��ʾѡ��
	}
	else {
		this->setStyleSheet(""); // ����߿�
	}
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
	isSelected = !isSelected;
	if (isSelected) {
		this->setStyleSheet("QLabel{border:2px solid rgb(0, 105, 246);}"); // ���ñ߿��ʾѡ��
	}
	else {
		this->setStyleSheet(""); // ����߿�
	}
}
