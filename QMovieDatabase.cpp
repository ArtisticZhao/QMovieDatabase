#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

#include "QMovieDatabase.h"



QMovieDatabase::QMovieDatabase(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	createTag = nullptr;
	// �����ļ���ȡ
	qDebug() << settings.dbPath << settings.playerPath;
	// ����DB
	dbHandler = new DBHandler(settings.dbPath);
	// ���� movie table ����
	movieTable = new MovieTable(ui.tableView, dbHandler);
	movieTable->bindingModel(dbHandler->getSqlQueryModel());
	foperator = new FileOperator(&settings);
	// ���� ��ǩɸѡ��
	tagFilter = new TagFilter(ui.listView, dbHandler, ui.rb_selectModeAnd);
	connect(dbHandler, SIGNAL(newTagAdded(QString, long)), tagFilter, SLOT(on_tagAdded(QString, long)));
	connect(movieTable, SIGNAL(tagEditTrigger(int, QStringList)), this, SLOT(on_tagEditTrigger(int, QStringList)));
	connect(movieTable, SIGNAL(runPlayerTrigger(QStringList)), foperator, SLOT(runPlayer(QStringList)));
	
}

void QMovieDatabase::on_actionOpenDir_triggered() {
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // Ĭ���ļ����� �ҵ��ĵ�
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull()) {
		// dir ����ѡ����ļ�·��
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