#pragma once
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>

class MovieTable : public QObject{
	Q_OBJECT
signals:
	void tagEditTrigger(int movieid, QStringList tags);  //˫�� ��ǩʱ�������ź�, ��mainwindow����
public slots:
	void on_tableView_double_clicked(const QModelIndex);
public:
	MovieTable(QTableView* tableview);
	void bindingModel(QSqlTableModel* model);
	void bindingModel(QSqlQueryModel* model);
private:
	QTableView* tableView;
};

