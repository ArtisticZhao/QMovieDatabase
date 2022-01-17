#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QMovieDatabase.h"
#include "Settings.h"
#include "FileOperator.h"
#include "DBHandler.h"
#include "MovieTable.h"
#include "TagFilter.h"
#include "CreateTagDiag.h"



class QMovieDatabase : public QMainWindow {
	Q_OBJECT

public:
	QMovieDatabase(QWidget* parent = Q_NULLPTR);
	~QMovieDatabase();
private:
	Ui::QMovieDatabaseClass ui;
	Settings settings;
	DBHandler* dbHandler;
	MovieTable* movieTable;
	TagFilter* tagFilter;
	CreateTagDiag* createTag;
	FileOperator* foperator;
	// ******* ²Û *******
public slots:
	void on_actionOpenDir_triggered();
	void on_actionPlayAll_triggered();
	void on_pb_addTag_clicked();
	void on_tagEditTrigger(int movieid, QStringList tags);
};
