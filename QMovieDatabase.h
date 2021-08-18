#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QMovieDatabase.h"

class QMovieDatabase : public QMainWindow
{
    Q_OBJECT

public:
    QMovieDatabase(QWidget *parent = Q_NULLPTR);

private:
    Ui::QMovieDatabaseClass ui;

// ******* ²Û *******
public slots:
	void on_actionOpenDir_triggered();
};
