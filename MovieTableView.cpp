#include "MovieTableView.h"
#include <QHeaderView>
#include "MovieTableColumIndex.h"
#include "StarDelegate.h"
#include "FileOperator.h"
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


void MovieTableView::on_menuClicked(QPoint pos) {
	contextMenu->exec(QCursor::pos());
}

void MovieTableView::on_actionPlay_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// 存在选中的对象, 如果多选则只重命名第一个
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		emit runPlayerTrigger(QStringList(this->model()->data(index).toString()));
	}
}

void MovieTableView::on_actionOpenFolder_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// 存在选中的对象, 如果多选则只重命名第一个
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		auto fullpath = this->model()->data(index).toString();
		FileOperator::openInFolder(fullpath);
	}
}

void MovieTableView::on_actionRename_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// 存在选中的对象, 如果多选则只重命名第一个
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Name));
		this->edit(index);
	}
}

void MovieTableView::on_actionDelete_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// 存在选中的对象
		auto selections = this->selectionModel()->selectedIndexes();
		for (auto& selectedIndex: selections){
			auto index = this->model()->index(selectedIndex.row(), static_cast<int>(MovieTableColumIndex::Path));
			auto fullpath = this->model()->data(index).toString();
			auto res = FileOperator::deleteFile(fullpath);
			qDebug() << res;
			qDebug() << FileOperator::isExistFile(fullpath);
			if (!FileOperator::isExistFile(fullpath)) {
				// 删除文件成功，或文件不存在，从数据库中移除记录
				index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::ID));
				auto movieid = this->model()->data(index).toInt();
				dbHandler->removeMovie(movieid);
			}
		}
	}
}

void MovieTableView::on_actionSelect_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// 存在选中的对象, 如果多选则只重命名第一个
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		auto oldpath = this->model()->data(index).toString();
		index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::ID));
		auto id = this->model()->data(index).toUInt();
		// 打开文件选择对话框来选择文件
		QString fullpath;
		fullpath = FileOperator::selectFile(FileOperator::getAbsDir(oldpath));
		qDebug() << fullpath;
		if (fullpath.length()>0) {
			dbHandler->updatePath(id, fullpath);
		}
	}
}

MovieTableView::MovieTableView(QWidget* parent /*= nullptr*/)
	:QTableView(parent) {
	this->dbHandler = nullptr;
	// 禁止双击等编辑模式，只能通过规定好的方式进行编辑
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// 设置右键菜单
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	contextMenu = new QMenu(this);
	auto play = new QAction("Play", contextMenu);
	auto openFolder = new QAction("Open Folder", contextMenu);
	auto renameMovie = new QAction("Rename	<F2>", contextMenu);
	auto deleteMovie = new QAction("Delete	<Del>", contextMenu);
	auto selectMovie = new QAction("Select Path", contextMenu);
	
	contextMenu->addAction(play);
	contextMenu->addAction(openFolder);
	contextMenu->addAction(renameMovie);
	contextMenu->addAction(deleteMovie);
	contextMenu->addAction(selectMovie);
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_menuClicked(QPoint)));   // 绑定右键菜单信号
	connect(play, SIGNAL(triggered()), this, SLOT(on_actionPlay_triggered()));
	connect(openFolder, SIGNAL(triggered()), this, SLOT(on_actionOpenFolder_triggered()));
	connect(renameMovie, SIGNAL(triggered()), this, SLOT(on_actionRename_triggered()));
	connect(deleteMovie, SIGNAL(triggered()), this, SLOT(on_actionDelete_triggered()));
	connect(selectMovie, SIGNAL(triggered()), this, SLOT(on_actionSelect_triggered()));
	connect(this, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_doubleClicked(const QModelIndex)));  // 绑定双击信号
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
	// 设置自动列宽
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
	this->horizontalHeader()->setSectionResizeMode(static_cast<int>(MovieTableColumIndex::Rank), QHeaderView::Fixed);
	this->setColumnWidth(static_cast<int>(MovieTableColumIndex::Rank), 52);
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
