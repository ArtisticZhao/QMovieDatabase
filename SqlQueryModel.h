#pragma once
#include <qsqlquerymodel.h>
// SqlQueryModel
// ��ԭ��QSqlQueryModel����������Զ����޸Ĺ���
// ֧�ֵ��޸��ֶΣ� - rank
class SqlQueryModel : public QSqlQueryModel {
	Q_OBJECT
signals:
	void movieRankUpdate(int movieid, int rank);
	void movieNamePathUpdate(int movieid, QString name, QString path);
public:
	using QSqlQueryModel::QSqlQueryModel;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role);
};

