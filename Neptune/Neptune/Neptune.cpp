#pragma once
#include "stdafx.h"
#include "Neptune.h"

CNeptune::CNeptune(QWidget *parent)
	: QMainWindow(parent)
	, m_pFileSystemModel(new QFileSystemModel)
{
	// Initialize by automatically compiled source
	m_NeptuneMain.setupUi(this);

	// Initialize window attributes
	QSettings setting("setting.ini", QSettings::IniFormat);
	restoreGeometry(setting.value("geometry").toByteArray());
	restoreState(setting.value("windowState").toByteArray());

	// Get default user's desktop path 
	QStringList list = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
	QString qsPath("");
	if (!list.isEmpty())
	{
		qsPath = list.at(0);
	}
	// Initialize others 
	initTreeView(qsPath);
}

CNeptune::~CNeptune()
{
	QSettings setting("setting.ini", QSettings::IniFormat);
	setting.setValue("geometry", saveGeometry());
	setting.setValue("windowState", saveState());
}

void CNeptune::initTreeView(QString& qsRootPath)
{
	const QModelIndex qmdlIndex = m_pFileSystemModel->setRootPath(qsRootPath);
	m_NeptuneMain.m_treeView->setModel(m_pFileSystemModel.get());
	m_NeptuneMain.m_treeView->setRootIndex(qmdlIndex);

	m_NeptuneMain.m_treeView->setIndentation(20);
	const QSize availableSizeDesktop = QApplication::desktop()->availableGeometry(m_NeptuneMain.m_treeView).size();


	m_NeptuneMain.m_treeView->resize(availableSizeDesktop);
	m_NeptuneMain.m_treeView->setColumnWidth(0, m_NeptuneMain.m_treeView->width() / 8);

	m_NeptuneMain.m_treeView->setWindowTitle(QObject::tr("Dir View"));

	QStringList filters;
	filters << "*.jpg" << ".jpeg";

	m_pFileSystemModel->setNameFilters(filters);
	m_pFileSystemModel->setNameFilterDisables(false);
}
