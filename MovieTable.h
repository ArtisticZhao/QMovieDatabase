#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>

class MovieTable : public QObject{
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //双击 标签时发出的信号, 由mainwindow接收
public slots:
	void on_tableView_double_clicked(const QModelIndex);
public:
	MovieTable(QTableView* tableview);
	void bindingModel(QSqlTableModel* model);
	void bindingModel(QSqlQueryModel* model);
private:
	QTableView* tableView;
};

