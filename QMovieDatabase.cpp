#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

#include "QMovieDatabase.h"
#include "FileOperator.h"

QMovieDatabase::QMovieDatabase(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	createTag = nullptr;
	// 配置文件读取
	qDebug() << settings.dbPath << settings.playerPath;
	// 加载DB
	dbHandler = new DBHandler(settings.dbPath);
	// 设置 movie table 界面
	movieTable = new MovieTable(ui.tableView);
	movieTable->bindingModel(dbHandler->getSqlQueryModel());
	// 设置 标签筛选器
	tagFilter = new TagFilter(ui.listView, dbHandler);
	connect(dbHandler, SIGNAL(newTagAdded(QString, long)), tagFilter, SLOT(on_tagAdded(QString, long)));
}

void QMovieDatabase::on_actionOpenDir_triggered() {
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // 默认文件夹在 我的文档
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull()) {
		// dir 即是选择的文件路径
		QList<QFileInfo> files = FileOperator::pathWalk(dir);
		dbHandler->addFilesToDB(files);
	}
}

void QMovieDatabase::on_pb_addTag_clicked() {
	if (createTag == nullptr) {
		createTag = new CreateTagDiag(this, dbHandler);
	}
	else {
		delete createTag;
		createTag = new CreateTagDiag(this, dbHandler);
	}
	
}

QMovieDatabase::~QMovieDatabase() {
	delete dbHandler;
	delete movieTable;
	delete tagFilter;
}