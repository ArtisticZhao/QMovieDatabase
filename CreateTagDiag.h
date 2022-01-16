#pragma once
#include <QtWidgets/QDialog>
#include "Ui_Dialog_createTag.h"
#include "DBHandler.h"

class CreateTagDiag : public QDialog {
	Q_OBJECT
public:
	CreateTagDiag(QWidget* parent, DBHandler* dbHandler);
	~CreateTagDiag();
private:
	Ui::Dialog_createTag ui;
	DBHandler* dbHandler;
private slots:
	void on_buttonBox_accepted();
};

