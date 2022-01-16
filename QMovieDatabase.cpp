#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

#include "QMovieDatabase.h"
#include "FileOperator.h"



QMovieDatabase::QMovieDatabase(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	// �����ļ���ȡ
	qDebug() << settings.dbPath << settings.playerPath;
	// ����DB
	dbHandler = new DBHandler(settings.dbPath);
	// ���� movie table ����
	movieTable = new MovieTable(ui.tableView);
	movieTable->bindingModel(dbHandler->getSqlQueryModel());
	// ���� ��ǩɸѡ��
	QStandardItemModel* model = new QStandardItemModel();
	QStandardItem* Item = new QStandardItem();
	Item->setCheckable(true);
	Item->setCheckState(Qt::Checked);
	Item->setText("test");
	model->setItem(0, Item);
	ui.listView->setModel(model);
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