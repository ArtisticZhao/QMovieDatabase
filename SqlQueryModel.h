#pragma once
#include <qsqlquerymodel.h>
// SqlQueryModel
// 再原有QSqlQueryModel基础上添加自定义修改功能
// 支持的修改字段： - rank
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

