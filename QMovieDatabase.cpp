#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>
#include "QMovieDatabase.h"
#include "FileOperator.h"

QMovieDatabase::QMovieDatabase(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

}

void QMovieDatabase::on_actionOpenDir_triggered()
{
	QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  // Ĭ���ļ����� �ҵ��ĵ�
	QString dir;
	dir = QFileDialog::getExistingDirectory(nullptr, QString(""), location);

	if (!dir.isNull())
	{
		// dir ����ѡ����ļ�·��
		QList<QFileInfo> files = FileOperator::pathWalk(dir);
		qDebug() << files;

	}
	else {
		//���ǵ�ȡ��
	}

	
}
