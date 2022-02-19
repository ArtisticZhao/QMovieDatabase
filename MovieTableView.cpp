#include "MovieTableView.h"
#include <QHeaderView>
#include "MovieTableColumIndex.h"
#include "StarDelegate.h"
#include "FileOperator.h"
#include <QDebug>

void MovieTableView::on_doubleClicked(const QModelIndex itemIndex) {
	if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Tag)) {
		// ˫����ǩ
		auto tagstr = this->model()->data(this->model()->index(itemIndex.row(), itemIndex.column())).toString();
		QStringList tags = tagstr.split(",");
		emit tagEditTrigger(this->model()->data(this->model()->index(itemIndex.row(), static_cast<int>(MovieTableColumIndex::ID))).toInt(), tags);
	}
	else if (itemIndex.column() == static_cast<int>(MovieTableColumIndex::Rank)) {
		// ˫��rank
		auto item = this->model()->flags(itemIndex);
		this->edit(itemIndex);
	}
	else {
		qDebug() << this->model()->data(this->model()->index(itemIndex.row(), itemIndex.column()));  // ��ǰ˫��
		// ����������
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
		// ����ѡ�еĶ���, �����ѡ��ֻ��������һ��
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		emit runPlayerTrigger(QStringList(this->model()->data(index).toString()));
	}
}

void MovieTableView::on_actionOpenFolder_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// ����ѡ�еĶ���, �����ѡ��ֻ��������һ��
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		auto fullpath = this->model()->data(index).toString();
		FileOperator::openInFolder(fullpath);
	}
}

void MovieTableView::on_actionRename_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// ����ѡ�еĶ���, �����ѡ��ֻ��������һ��
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Name));
		this->edit(index);
	}
}

void MovieTableView::on_actionDelete_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// ����ѡ�еĶ���
		auto selections = this->selectionModel()->selectedIndexes();
		for (auto& selectedIndex: selections){
			auto index = this->model()->index(selectedIndex.row(), static_cast<int>(MovieTableColumIndex::Path));
			auto fullpath = this->model()->data(index).toString();
			auto res = FileOperator::deleteFile(fullpath);
			qDebug() << res;
			qDebug() << FileOperator::isExistFile(fullpath);
			if (!FileOperator::isExistFile(fullpath)) {
				// ɾ���ļ��ɹ������ļ������ڣ������ݿ����Ƴ���¼
				index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::ID));
				auto movieid = this->model()->data(index).toInt();
				dbHandler->removeMovie(movieid);
			}
		}
	}
}

void MovieTableView::on_actionSelect_triggered() {
	if (this->selectionModel()->hasSelection()) {
		// ����ѡ�еĶ���, �����ѡ��ֻ��������һ��
		auto selections = this->selectionModel()->selectedIndexes();
		auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Path));
		auto oldpath = this->model()->data(index).toString();
		index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::ID));
		auto id = this->model()->data(index).toUInt();
		// ���ļ�ѡ��Ի�����ѡ���ļ�
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
	// ��ֹ˫���ȱ༭ģʽ��ֻ��ͨ���涨�õķ�ʽ���б༭
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// �����Ҽ��˵�
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
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_menuClicked(QPoint)));   // ���Ҽ��˵��ź�
	connect(play, SIGNAL(triggered()), this, SLOT(on_actionPlay_triggered()));
	connect(openFolder, SIGNAL(triggered()), this, SLOT(on_actionOpenFolder_triggered()));
	connect(renameMovie, SIGNAL(triggered()), this, SLOT(on_actionRename_triggered()));
	connect(deleteMovie, SIGNAL(triggered()), this, SLOT(on_actionDelete_triggered()));
	connect(selectMovie, SIGNAL(triggered()), this, SLOT(on_actionSelect_triggered()));
	connect(this, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(on_doubleClicked(const QModelIndex)));  // ��˫���ź�
}

void MovieTableView::set_dbHandler(DBHandler* dbHandler) {
	this->dbHandler = dbHandler;
}

void MovieTableView::bindingModel(SqlQueryModel* model) {
	this->setModel(model);
	// ����id��
	this->setColumnHidden(0, true);
	// �����Ǽ���ʾ
	this->setItemDelegateForColumn(3, new StarDelegate);
	// �����Զ��п�
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//�����ж���չ����Ӧ��ȣ�������������Ļ���
	this->horizontalHeader()->setSectionResizeMode(static_cast<int>(MovieTableColumIndex::Rank), QHeaderView::Fixed);
	this->setColumnWidth(static_cast<int>(MovieTableColumIndex::Rank), 52);
	// �󶨸����ź�
	connect(model, SIGNAL(movieRankUpdate(int, int)), this, SLOT(on_movieRankUpdate(int, int)));
	connect(model, SIGNAL(movieNamePathUpdate(int, QString, QString)), this, SLOT(on_movieNamePathUpdate(int, QString, QString)));
}

void MovieTableView::runPlayer(QStringList qs) {
	// ����������
	emit runPlayerTrigger(qs);
}

void MovieTableView::keyPressEvent(QKeyEvent* event) {
	// ������ӰƬ
	if (event->key() == Qt::Key_F2) {
		if (this->selectionModel()->hasSelection()) {
			// ����ѡ�еĶ���, �����ѡ��ֻ��������һ��
			auto selections = this->selectionModel()->selectedIndexes();
			auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Name));
			this->edit(index);
		}
	}
}
