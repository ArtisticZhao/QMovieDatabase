#include "MovieTableView.h"
#include "MovieTableColumIndex.h"
#include <QDebug>

MovieTableView::MovieTableView(QWidget* parent /*= nullptr*/)
	:QTableView(parent) {
}

void MovieTableView::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_F2) {
		// 重命名影片		
		if (this->selectionModel()->hasSelection()) {
			// 存在选中的对象
			auto selections = this->selectionModel()->selectedIndexes();
			auto index = this->model()->index(selections.at(0).row(), static_cast<int>(MovieTableColumIndex::Name));
			this->edit(index);
		}
	}
}
