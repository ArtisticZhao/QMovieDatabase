#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>
#include "QMovieDatabase.h"
#include "FileOperator.h"

QMovieDatabase::QMovieDatabase(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);

	qDebug() << settings.dbPath << settings.playerPath;
	dbHandler = new DBHandler(settings.dbPath);
	dbHandler->createTable();
	qDebug() <<  dbHandler->isTableExist(QString("t_movie"));

}

void QMovieDatabase::on_actionOpenDir_triggered() {
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // 默认文件夹在 我的文档
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull()) {
		// dir 即是选择的文件路径
		QList<QFileInfo> files = FileOperator::pathWalk(dir);
		qDebug() << files;
	}
}

QMovieDatabase::~QMovieDatabase() {
	delete dbHandler;
}