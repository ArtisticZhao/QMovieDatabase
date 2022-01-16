#include "CreateTagDiag.h"
#include <QDebug>
CreateTagDiag::CreateTagDiag(QWidget* parent, DBHandler* dbHandler)
	: QDialog(parent) {
	ui.setupUi(this);
	this->show();
	this->dbHandler = dbHandler;
}

CreateTagDiag::~CreateTagDiag() {

}

void CreateTagDiag::on_buttonBox_accepted() {
	qDebug() << ui.lineEdit->text();
	dbHandler->createTag(ui.lineEdit->text());
}
