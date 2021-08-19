#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QMovieDatabase.h"
#include "Settings.h"
#include "DBHandler.h"
class QMovieDatabase : public QMainWindow {
	Q_OBJECT

public:
	QMovieDatabase(QWidget* parent = Q_NULLPTR);
	~QMovieDatabase();
private:
	Ui::QMovieDatabaseClass ui;
	Settings settings;
	DBHandler* dbHandler;
	// ******* �� *******
public slots:
	void on_actionOpenDir_triggered();
};
