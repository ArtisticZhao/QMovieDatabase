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
	qDebug() <<  dbHandler->isTableExist(QString("t_movies"));
	movieTable = new MovieTable(ui.tableView);
	movieTable->bindingModel(dbHandler->getSqlQueryModel());
}

void QMovieDatabase::on_actionOpenDir_triggered() {
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // Ĭ���ļ����� �ҵ��ĵ�
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull()) {
		// dir ����ѡ����ļ�·��
		QList<QFileInfo> files = FileOperator::pathWalk(dir);
		dbHandler->addFilesToDB(files);
	}
}

QMovieDatabase::~QMovieDatabase() {
	delete dbHandler;
}