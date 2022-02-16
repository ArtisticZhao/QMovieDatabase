#pragma once
#include <qtableview.h>
#include <QKeyEvent>

class MovieTableView : public QTableView {
public:
	explicit MovieTableView(QWidget* parent = nullptr);
protected:
	virtual void keyPressEvent(QKeyEvent* event);
};

