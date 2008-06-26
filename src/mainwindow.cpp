/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała   *
 *   b@kontrasty.szczecin.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QtGui>
#include "mainwindow.h"

class DownloadViewDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	inline DownloadViewDelegate(MainWindow* parent) : QItemDelegate(parent){};
	inline void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index )
	{
		if (index.column() != 2) 
		{
			QItemDelegate::paint(painter, option, index);
			return;
		}
		QStyleOptionProgressBar progressBarOption;
		progressBarOption.state = QStyle::State_Enabled;
		progressBarOption.direction = QApplication::layoutDirection();
		progressBarOption.rect = option.rect;
		progressBarOption.fontMetrics = QApplication::fontMetrics();
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.textAlignment = Qt::AlignCenter;
		progressBarOption.textVisible = true;	
		// Set the progress and text values of the style option.
		int progress = qobject_cast<MainWindow *>( parent())->GetProgress();
		progressBarOption.progress = progress < 0 ? 0 : progress;
		progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
		//Draw the progress bar onto the view.
		QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
	}
};

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), m_apSettings(new QSettings(QSettings::UserScope,"fsf", APPLICATION_NAME)),
	m_RapidshareDownloadManager(new RapidShareDownloadManager() )
{
	QT_DEBUG_FUNCTION
	m_ColumnHeaders << "File Path" << "Where " << "Progress" << "Download rate " << "Status ";
	m_bExit = false;
	m_DownloadView =  new DownloadView(this) ;
	m_DownloadView->setItemDelegate( new DownloadViewDelegate(this) );
	m_DownloadView->setHeaderLabels( m_ColumnHeaders );
	m_DownloadView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_DownloadView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_DownloadView->setAlternatingRowColors( true );
	m_DownloadView->setRootIsDecorated( false );
	m_MenuBar = new QMenuBar(this);
	m_FileMenu = new QMenu( tr("&File"), m_MenuBar);
	m_File_NewAction = new QAction(tr( "&New" ), m_FileMenu) ;
	m_File_SendToTrayAction = new QAction(tr( "Send to &Tray" ), m_FileMenu) ;
	m_File_ExitAction = new QAction(tr( "E&xit" ), m_FileMenu) ;
	m_SettingsMenu = new QMenu(tr("&Settings"), m_MenuBar);
	m_Settings_Configure = new QAction(tr("Configure"), m_SettingsMenu);
	InitializeSystemTray();
	ConnectActions();
	SetupUi();
	ReadSettings();
};
MainWindow::~MainWindow()
{
	QT_DEBUG_FUNCTION
	WriteSettings();
	DeInitialize();
};
void MainWindow::InitializeSystemTray()
{
	QT_DEBUG_FUNCTION
	if( !QSystemTrayIcon::isSystemTrayAvailable() )
	{
		DebugUtils::q_Warn(tr("No systemtray found") );
		m_apIsSystemTray.reset(new bool(false));
	}
	m_SystemTrayIcon = new QSystemTrayIcon(this);
	m_SystemTrayMenu = new QMenu(this);
	// actions
	m_STHideAction = new QAction(tr("&Hide"), this);
	m_STRestoreAction = new QAction(tr("&Restore"), this);
	m_STQuitAction = new QAction(tr("&Quit"), this);
	connect(m_STHideAction, SIGNAL(triggered()), this, SLOT(hide()));
	connect(m_STRestoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	connect(m_STQuitAction, SIGNAL(triggered()), this, SLOT(close()));
	m_SystemTrayMenu->addAction(m_STHideAction);
	m_SystemTrayMenu->addAction(m_STRestoreAction);
	m_SystemTrayMenu->addAction(m_STQuitAction);
	
	m_SystemTrayIcon->setContextMenu( m_SystemTrayMenu );
	m_SystemTrayIcon->setIcon( QIcon( ":/icon_systray.png") );
	m_SystemTrayIcon->show();
	
}
void MainWindow::ConnectActions()
{
	QT_DEBUG_FUNCTION
	
	// menu actions and slots
	QObject::connect(m_File_ExitAction, SIGNAL(triggered()), this, SLOT(close() ) );
	QObject::connect(m_File_NewAction, SIGNAL(triggered()), this, SLOT(addNewFile()));
	QObject::connect(m_File_SendToTrayAction, SIGNAL(triggered()), this, SLOT(hide()));
	QObject::connect(m_Settings_Configure, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
	// system tray actions and slots
	QObject::connect(m_SystemTrayIcon,SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT(Activation( QSystemTrayIcon::ActivationReason )));
	// RapidshareDownloadManager actions and slots
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( DoneDownloading( unsigned int ) ), this, SLOT( DoneDownloading( const unsigned int ) ) );
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( ChangedProgress( unsigned int,unsigned int ) ), 
	this, SLOT( ChangeProgressValue( const unsigned int, const unsigned int  ) ) );
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( ChangedName( unsigned int, QString  ) ), 
	this, SLOT( ChangeProgressName( unsigned int,QString ) ) );
}
void MainWindow::SetupUi()
{
	QT_DEBUG_FUNCTION
	m_FileMenu->addAction( m_File_NewAction );
	m_FileMenu->addSeparator();
	m_FileMenu->addAction( m_File_SendToTrayAction );
	m_FileMenu->addAction( m_File_ExitAction );
	m_MenuBar->addMenu(m_FileMenu);
	m_SettingsMenu->addAction(m_Settings_Configure);
	m_MenuBar->addMenu(m_SettingsMenu);
	setCentralWidget( m_DownloadView );
	setMenuBar(m_MenuBar);
}
bool MainWindow::addFileToDownload(const QString & fileToDownload)
{
	QT_DEBUG_FUNCTION
	QRapidshareUser rsUser = m_RapidshareDownloadManager->GetUser();
	if( rsUser.getUserName().isEmpty() ||  rsUser.getUserPass().isEmpty() )
	{
		DebugUtils::q_Error("RS user not set ! ");
		return false ;
	}
		
	QString fileUrl;
	QString dest;
	QString baseFileName;
	QUrl FileUrl;
	if(fileToDownload.isEmpty())
	{
		Ui_AddDownloadFile *dlg = new Ui_AddDownloadFile(this, fileToDownload) ; 
		if( ! dlg->exec() )
			return 0;
		FileUrl = dlg->GetUrl();
		if( FileUrl.path().isEmpty() )
		{
			QMessageBox::warning(this, tr("Error"),
			tr("The file %1 cannot not be opened/resumed.").arg(FileUrl.toString()));
		}
		QString dest = dlg->GetDestinationPath();
		baseFileName = QFileInfo(FileUrl.path()).fileName();	
	}
	else
	{
		FileUrl = QUrl(fileToDownload);
		dest = QDir::home().path();
	}
	dest += "/";
	QTreeWidgetItem *item = new QTreeWidgetItem(m_DownloadView);
	item->setText(0,FileUrl.path());
	item->setText(1,dest);
	item->setText(2,tr("0/0"));
	item->setText(3,tr("0 kBps"));
	item->setText(4,tr("Initializing"));
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	item->setTextAlignment(1, Qt::AlignHCenter);
	m_DownloadView->setCurrentItem(item);
	m_RapidsharePoolView.push_back(item);
	QString fileName = dest + TransformUrlPathToLocalPath( FileUrl.path() );
	m_RapidshareDownloadManager->AddDownload(FileUrl.toString(), fileName);
	return true;
}
/*
 * 
 * SLOTS
 * 
 */
void MainWindow::addNewFile()
{
	QT_DEBUG_FUNCTION
	addFileToDownload();
};
void MainWindow::showConfigurationDialog()
{
	QT_DEBUG_FUNCTION
	ConfigurationDialog *dialog = new ConfigurationDialog(this);
	dialog->exec();
	
}
void MainWindow::keyPressEvent(QKeyEvent *keyPressed)
{
	QT_DEBUG_EVENT
	if(keyPressed->modifiers() == Qt::ControlModifier)
	{
		if ( keyPressed->key() == Qt::Key_V )
		{
			// segment for several download files
			QClipboard *clipboard = QApplication::clipboard ();
			QString text = clipboard->text(QClipboard::Clipboard);
			QStringList urls = text.split( QRegExp( "\\s+" ) );
			qDebug() << urls;
			foreach(QString one, urls)
			{
				if(one.contains("rapidshare"))
					addFileToDownload(one);	
			}
		}
	}
}
void MainWindow::closeEvent(QCloseEvent * event)
{
	QT_DEBUG_EVENT
	qDebug() << m_bExit;
	if(! m_bExit )
	{
		event->ignore();
		hide();
	}
}
void MainWindow::hideEvent(QHideEvent *event)
{
	QT_DEBUG_EVENT
	SaveUiSettings();
}
void MainWindow::showEvent(QShowEvent *event)
{
	QT_DEBUG_EVENT
	LoadUiSettings();
};
void MainWindow::Activation(QSystemTrayIcon::ActivationReason reason)
{
	QT_DEBUG_FUNCTION
	if(reason != QSystemTrayIcon::Context)
	{
		if(this->isVisible())
			hide();
		else if(this->isHidden())
			showNormal();
	}
};
void MainWindow::close()
{
	QT_DEBUG_FUNCTION
	m_bExit = true; // close chłopaku
	QMainWindow::close();
}

void MainWindow::ChangeProgressName( const unsigned int & at, const QString & name ) 
{
	QT_DEBUG_FUNCTION
	QTreeWidgetItem *hadzia = m_RapidsharePoolView.at( at ); 
	if(NULL == hadzia)
		return;
	else
		hadzia->setText(4,name); 
	
}
void MainWindow::ChangeProgressValue(const unsigned int & at ,  const unsigned int & iPerc ) 
{
	QT_DEBUG_FUNCTION
	QTreeWidgetItem *hadzia = m_RapidsharePoolView.at( at );  
	if( NULL == hadzia ) 
		return;
	else
	{
		hadzia->setText(2, QString::number(iPerc));
	}
	m_progress = iPerc;
}
void MainWindow::DoneDownloading(const unsigned int & at )
{
	QT_DEBUG_FUNCTION
}
QString MainWindow::TransformUrlPathToLocalPath(const QString & url)
{
	QT_DEBUG_FUNCTION
	QString ret = QString(url);
	ret = ret.remove("/files");
	// remove /213123123/ <- digits only
	ret = ret.remove(QRegExp ("/\\d+/") );
	return ret;
}
DownloadView::DownloadView(QWidget * parent) : QTreeWidget( parent ) 
{
	QT_DEBUG_FUNCTION
	setAcceptDrops(true);
};
void DownloadView::dropEvent(QDropEvent *event)
{
	QString fileName = QUrl(event->mimeData()->text()).path();
	if (QFile::exists(fileName) && fileName.toLower().endsWith(".torrent"))
		emit fileDropped(fileName);
}
void DownloadView::dragMoveEvent(QDragMoveEvent * event)
{
	QUrl url(event->mimeData()->text());
// 	if (url.isValid() && url.scheme().toLower() == "file"
// 		   && url.path().toLower().endsWith(".torrent"))
	event->acceptProposedAction();
}

void MainWindow::SetUser(const QString & userName, const QString & userPass)
{
	QT_DEBUG_FUNCTION
	
	m_RapidshareDownloadManager->SetUser( QRapidshareUser( userName , userPass ) );
}

void MainWindow::ReadSettings()
{
	QT_DEBUG_FUNCTION
	QString userName = m_apSettings->value(SET_USERNAME).toString();
	QString userPass = m_apSettings->value(SET_USERPASSWORD).toString();
	bool bOk;
	int 	maxDownl = m_apSettings->value(RSM_MAX_DOWNLOAD).toInt(&bOk);
	if( userName.isEmpty() || userPass.isEmpty() )
	{
		DebugUtils::q_Error("Unable to read user or password ");
		// do U want to set it now ? 
	}
	SetUser(userName, userPass);
	LoadUiSettings();
	if(bOk && maxDownl != 0)
		m_RapidshareDownloadManager->SetMaxDownloads(maxDownl);
	else
		m_RapidshareDownloadManager->SetMaxDownloads(3);
};

void MainWindow::WriteSettings()
{
	QT_DEBUG_FUNCTION
	QRapidshareUser rsUser = m_RapidshareDownloadManager->GetUser();
	m_apSettings->setValue( SET_USERNAME,rsUser.getUserName() );
	m_apSettings->setValue( SET_USERPASSWORD,rsUser.getUserPass() );
	SaveUiSettings();
	m_apSettings->setValue( RSM_MAX_DOWNLOAD,m_RapidshareDownloadManager->GetMaxDownloads() );
	m_apSettings->sync();
};
void MainWindow::SaveUiSettings()
{
	m_apSettings->setValue(UI_WINDOW_POS, pos());
	m_apSettings->setValue(UI_WINDOW_SIZE, size());
}
void MainWindow::LoadUiSettings()
{
	QPoint pos = m_apSettings->value(UI_WINDOW_POS).toPoint();
	move(pos);
	QSize size =  m_apSettings->value(UI_WINDOW_SIZE).toSize();
	resize(size);
}
void MainWindow::DeInitialize()
{
	QT_DEBUG_FUNCTION
	m_apIsSystemTray.release();
	m_apSettings.release();
	foreach(QTreeWidgetItem* tmp, m_RapidsharePoolView)
		delete tmp;
}
#include "mainwindow.moc"
