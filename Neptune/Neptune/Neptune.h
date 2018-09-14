#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Neptune.h"

class CNeptune : public QMainWindow
{
	Q_OBJECT

public:
	CNeptune(QWidget *parent = Q_NULLPTR);
	~CNeptune();

	void initTreeView(QString& qstrRootPath);

private slots:
	void slotTreeViewExpanded(QModelIndex qIndex);

private:
	Ui::CNeptuneMainWindow m_NeptuneMain;
	std::unique_ptr<QFileSystemModel> m_pFileSystemModel;

};
