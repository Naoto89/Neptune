#pragma once
#include "stdafx.h"
#include "Neptune.h"

CNeptune::CNeptune(QWidget *parent)
	: QMainWindow(parent)
	, m_pFileSystemModel(new QFileSystemModel)
{
	// Initialize by automatically compiled source
	m_NeptuneMain.setupUi(this);
	
	// Setting window size/position dynamically
	QSize desktopSize = QApplication::desktop()->size();
	QSize resSize1 = desktopSize*0.9;
	QSize resSize2 = desktopSize*0.05;
	this->resize(resSize1);
	this->setGeometry(QRect(resSize2.width(), resSize2.height(), resSize1.width(), resSize1.height()));


	// Initialize Log Widget 
	QString path("C:\\Users\\naoto\\Desktop\\CodingRepository\\trunk\\Neptune\\Neptune\\Neptune\\StyleSheet\\StyleSheet_BaseSetting.qss");
	QFile file(path);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	QString inStr = in.readAll();
	file.close();

	//m_NeptuneMain.m_dockWidgetLog->setStyleSheet(path);
	m_NeptuneMain.m_textEditLog->setStyleSheet(inStr);


	// Initialize window attributes
	QSettings setting("setting.ini", QSettings::IniFormat);
	restoreGeometry(setting.value("geometry").toByteArray());
	restoreState(setting.value("windowState").toByteArray());

	// Initialize others 

	// Get default user's desktop path 
	QStringList list = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
	if (!list.isEmpty())
	{
		//initTreeView(QString(list.at(0)));
		initTreeView(QString("C:\\Users\\Share\\Dropbox"));
		/*C:\Users\Share\Dropbox*/
	}


	m_NeptuneMain.m_listWidget->setViewMode(QListWidget::IconMode);
	m_NeptuneMain.m_listWidget->setIconSize(QSize(200, 200));
	m_NeptuneMain.m_listWidget->setResizeMode(QListWidget::Fixed);
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
	filters << "*.jpg" << ".jpeg" << "*.bmp";

	m_pFileSystemModel->setNameFilters(filters);
	m_pFileSystemModel->setNameFilterDisables(false);
}


/*===================================================
				Slot Definiion
 ===================================================*/

void CNeptune::slotTreeViewExpanded(QModelIndex qIndex)
{
		
	if (m_pFileSystemModel->isDir(qIndex))
	{
		// Search file path
		QString qsFolderPath = m_pFileSystemModel->filePath(qIndex);
		QStringList filters;
		filters << "*.jpg" << ".jpeg" << "*.bmp";
		QDirIterator it(qsFolderPath, filters, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		QStringList files;
		while (it.hasNext())
		{
			files << it.next();
		}
		files.sort();

		if (qsFolderPath == m_qsLastOpenedPath || files.length() == 0)
		{
			return;
		}
		// Save last path
		m_qsLastOpenedPath = qsFolderPath;

		// Clear all thumbnail
		m_NeptuneMain.m_listWidget->clear();

		int64_t totalTime = 0;
		for (auto file : files)
		{
			QElapsedTimer timer;
			timer.start();

			QImageReader reader(file);
			reader.setQuality(0);
			QSize qs = reader.size();
			QRect rect;

			if (qs.width() > qs.height())
			{
				int diff = (qs.width() - qs.height()) / 2;
				rect.setRect(diff, 0, qs.height(), qs.height());
			}
			else
			{
				int diff = (qs.height() - qs.width()) / 2;
				rect.setRect(0, diff, qs.width(), qs.width());
			}

			reader.setClipRect(rect);
			reader.setScaledSize(QSize(200, 200));

			// Open and get image data with thumbnail size
			QImage img = reader.read();

			QString name = QFileInfo(file).fileName();
			if (name.length() >= 20)
			{
				name = name.left(17) + "...";
			}

			// Add a thumbnail
			m_NeptuneMain.m_listWidget->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(img)), name));
			
			int64_t elapsedTime = timer.elapsed();
			totalTime += elapsedTime;

			// Output Log
			QString qsLog; qsLog.sprintf("[%s] :Load time %-20s : %6d [ms]"
				, QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().constData()
				, name.toUtf8().constData()
				, elapsedTime);
			m_NeptuneMain.m_textEditLog->append(qsLog);
		}

		// Output Log
		m_NeptuneMain.m_textEditLog->append("==========================================");
		QString qsLog2; qsLog2.sprintf("Load total time : %6d [ms]", totalTime);
		m_NeptuneMain.m_textEditLog->append(qsLog2);
		m_NeptuneMain.m_textEditLog->append("==========================================");
	}
}

void CNeptune::slotLogClear()
{
	m_NeptuneMain.m_textEditLog->clear();
}
