#include "SqlQueryModel.h"
#include "MovieTableColumIndex.h"
#include "FileOperator.h"
#include <qdebug>

Qt::ItemFlags SqlQueryModel::flags(const QModelIndex& index) const {
	// 设置rank字段可编辑
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
	}
	// 设置name字段可编辑
	if (index.column() == static_cast<int>(MovieTableColumIndex::Name)) {
		return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
	}
	return QSqlQueryModel::flags(index);
}

bool SqlQueryModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	// 修改rank之后保存
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		// 更新rank分
		// 获取movie id
		auto movieid = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt();
		emit movieRankUpdate(movieid, value.toInt());
		return true;
	}
	// 修改name之后的操作：更新文件名，更新数据库中对应的path
	if (index.column() == static_cast<int>(MovieTableColumIndex::Name)) {
		auto oldNameStr = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::Name))).toString();
		auto name = value.toString();
		if (QString::compare(oldNameStr, name) == 0) {
			// 没有发生变更
			return false;
		}
		// 1. 检查更改后的文件名是否符合文件名规则
		bool islegal = FileOperator::isLegalFileName(name);
		if (!islegal) {
			return false;
		}
		// 2. 修改文件名
		auto fullpath = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::Path))).toString();
		auto newpath = FileOperator::renameFile(fullpath, name);
		// 3. 修改对应数据库中path路径
		auto movieid = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt();
		emit movieNamePathUpdate(movieid, name, newpath);
		return true;
	}
	return false;
}
