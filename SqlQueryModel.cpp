#include "SqlQueryModel.h"
#include "MovieTableColumIndex.h"
#include <qdebug>

Qt::ItemFlags SqlQueryModel::flags(const QModelIndex& index) const {
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
	}
	return QSqlQueryModel::flags(index);
}

bool SqlQueryModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		// 更新rank分
		// 获取movie id
		auto movieid = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt();
		emit movieRankUpdate(movieid, value.toInt());
	}
	return true;
}
