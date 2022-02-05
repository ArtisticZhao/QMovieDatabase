#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

#include "QMovieDatabase.h"



QMovieDatabase::QMovieDatabase(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	createTag = nullptr;
	// 配置文件读取
	qDebug() << settings.dbPath << settings.playerPath;
	// 加载DB
	dbHandler = new DBHandler(settings.dbPath);
	// 设置 movie table 界面
	movieTable = new MovieTable(ui.tableView, dbHandler);
	movieTable->bindingModel(dbHandler->getSqlQueryModel());
	foperator = new FileOperator(&settings);
	// 设置 标签筛选器
	tagFilter = new TagFilter(ui.listView, dbHandler, ui.rb_selectModeAnd);
	connect(dbHandler, SIGNAL(newTagAdded(QString, long)), tagFilter, SLOT(on_tagAdded(QString, long)));
	connect(movieTable, SIGNAL(tagEditTrigger(int, QStringList)), this, SLOT(on_tagEditTrigger(int, QStringList)));
	connect(movieTable, SIGNAL(runPlayerTrigger(QStringList)), foperator, SLOT(runPlayer(QStringList)));
	
}

void QMovieDatabase::on_actionOpenDir_triggered() {
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // 默认文件夹在 我的文档
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull()) {
		// dir 即是选择的文件路径
		QList<QFileInfo> files = foperator->pathWalk(dir);
		dbHandler->addFilesToDB(files);
	}
}

void QMovieDatabase::on_actionPlayAll_triggered() {
	QStringList paths;
	for (int i=0 ;i<ui.tableView->model()->rowCount(); i++)
	{
		/*QString onepath;
		onepath.append('"');
		onepath.append(ui.tableView->model()->data(ui.tableView->model()->index(i, 3)).toString());
		onepath.append('"');*/
		paths << ui.tableView->model()->data(ui.tableView->model()->index(i, 3)).toString();
	}
	movieTable->runPlayer(paths);
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

void QMovieDatabase::on_tagEditTrigger(int movieid, QStringList tags) {
	tagFilter->editTags(this, movieid, tags);
}

QMovieDatabase::~QMovieDatabase() {
	delete dbHandler;
	delete movieTable;
	delete tagFilter;
	delete foperator;
}