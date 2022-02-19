#pragma once
#include <qtableview.h>
#include <QKeyEvent>
#include "DBHandler.h"
#include "SqlQueryModel.h"

class MovieTableView : public QTableView {
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //双击 标签时发出的信号, 由mainwindow接收
	void runPlayerTrigger(QStringList paths);
public slots:
	void on_doubleClicked(const QModelIndex);
	void on_movieRankUpdate(int movieid, int rank);
	void on_movieNamePathUpdate(int movieid, QString name, QString path);
public:
	explicit MovieTableView(QWidget* parent = nullptr);
	void set_dbHandler(DBHandler* dbHandler);
	void bindingModel(SqlQueryModel* model);
	void runPlayer(QStringList qs);
protected:
	virtual void keyPressEvent(QKeyEvent* event);
private:
	DBHandler* dbHandler;
};

