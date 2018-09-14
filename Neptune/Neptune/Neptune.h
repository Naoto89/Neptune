#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Neptune.h"

class Neptune : public QMainWindow
{
	Q_OBJECT

public:
	Neptune(QWidget *parent = Q_NULLPTR);

private:
	Ui::NeptuneClass ui;
};
