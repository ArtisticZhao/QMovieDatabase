#include "SqlQueryModel.h"
#include "MovieTableColumIndex.h"
#include "FileOperator.h"
#include <qdebug>

Qt::ItemFlags SqlQueryModel::flags(const QModelIndex& index) const {
	// ����rank�ֶοɱ༭
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
	}
	// ����name�ֶοɱ༭
	if (index.column() == static_cast<int>(MovieTableColumIndex::Name)) {
		return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;
	}
	return QSqlQueryModel::flags(index);
}

bool SqlQueryModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	// �޸�rank֮�󱣴�
	if (index.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		// ����rank��
		// ��ȡmovie id
		auto movieid = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt();
		emit movieRankUpdate(movieid, value.toInt());
		return true;
	}
	// �޸�name֮��Ĳ����������ļ������������ݿ��ж�Ӧ��path
	if (index.column() == static_cast<int>(MovieTableColumIndex::Name)) {
		auto oldNameStr = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::Name))).toString();
		auto name = value.toString();
		if (QString::compare(oldNameStr, name) == 0) {
			// û�з������
			return false;
		}
		// 1. �����ĺ���ļ����Ƿ�����ļ�������
		bool islegal = FileOperator::isLegalFileName(name);
		if (!islegal) {
			return false;
		}
		// 2. �޸��ļ���
		auto fullpath = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::Path))).toString();
		auto newpath = FileOperator::renameFile(fullpath, name);
		// 3. �޸Ķ�Ӧ���ݿ���path·��
		auto movieid = this->data(this->index(index.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt();
		emit movieNamePathUpdate(movieid, name, newpath);
		return true;
	}
	return false;
}
