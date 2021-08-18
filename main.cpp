#include "QMovieDatabase.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMovieDatabase w;
    w.show();
    return a.exec();
}
