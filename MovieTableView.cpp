#include "MovieTableView.h"
#include "MovieTableColumIndex.h"
#include "StarDelegate.h"
#include <QDebug>

void MovieTableView::on_doubleClicked(const QModelIndex itemIndex) {
	if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Tag)) {
		// 双击标签
		auto tagstr = this->model()->data(this->model()->index(itemIndex.row(), itemIndex.column())).toString();
		QStringList tags = tagstr.split(",");
		emit tagEditTrigger(this->model()->data(this->model()->index(itemIndex.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt(), tags);
	}
	else if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		// 双击rank
		auto item = this->model()->flags(itemIndex);
		this->edit(itemIndex);
	}
	else {
		qDebug() << this->model()->data(this->model()->index(itemIndex.row(), itemIndex.column()));  // 当前双击
		// 启动播放器
		emit runPlayerTrigger(QStringList(this->model()->data(this->model()->index(itemIndex.row(), static_cast<int>(MovieTableColumIndex::Path))).toString()));
	}
}

void MovieTableView::on_movieRankUpdate(int movieid, int rank) {
	if (dbHandler != nullptr) {
		dbHandler->updateRank(movieid, rank);
	}
	else {
		qDebug() << "dbHandler not set in MovieTableView";
	}
}

void MovieTableView::on_movieNamePathUpdate(int movieid, QString name, QString path) {
	if (dbHandler != nullptr) {
		dbHandler->updateNamePath(movieid, name, path);
	}
	else {
		qDebug() << "dbHandler not set in MovieTableView";
	}
}

MovieTableView::MovieTableView(QWidget* parent /*= nullptr*/)
	:QTableView(parent) {
	this->dbHandler = nullptr;
	// 禁止双击等编辑模式，只能通过规定好的方式进行编辑
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	connect(this, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_doubleClicked(const QModelIndex)));
}

void MovieTableView::set_dbHandler(DBHandler* dbHandler) {
	this->dbHandler = dbHandler;
}

void MovieTableView::bindingModel(SqlQueryModel* model) {
	this->setModel(model);
	// 隐藏id列
	this->setColumnHidden(0, true);
	// 设置星级显示
	this->setItemDelegateForColumn(3, new StarDelegate);
	// 绑定更新信号
	connect(model, SIGNAL(movieRankUpdate(int, int)), this, SLOT(on_movieRankUpdate(int, int)));
	connect(model, SIGNAL(movieNamePathUpdate(int, QString, QString)), this, SLOT(on_movieNamePathUpdate(int, QString, QString)));
}

void MovieTableView::runPlayer(QStringList qs) {
	// 启动播放器
	emit runPlayerTrigger(qs);
}

void MovieTableView::keyPressEvent(QKeyEvent* event) {
	// 重命名影片
	if (event->key() == Qt::Key_F2) {
		if (this->selectionModel()->hasSelection()) {
			// 存在选中的对象, 如果多选则只重命名第一个
			auto selections = this->selectionModel()->selectedIndexes();
			auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Name));
			this->edit(index);
		}
	}
}
