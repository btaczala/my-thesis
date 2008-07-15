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
#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), m_apSettings(new QSettings(QSettings::UserScope,"fsf", APPLICATION_NAME)),
	m_RapidshareDownloadManager(new RapidShareDownloadManager() ), m_Logger(" mainwindow "), m_ContextMenuOnItem(0)
{
	RSDM_LOG_FUNC ;
	m_ColumnHeaders << "File Path" << "Where " << "Progress" << "Download rate " << "Status ";
	m_bExit = false;
	/*
		View
	*/
	m_DownloadView =  new DownloadView(this) ;
	//m_apDownloadDelegate.reset( new DownloadViewDelegate(m_DownloadView));
	m_DownloadView->setItemDelegate(new DownloadViewDelegate(this));
	m_DownloadView->setHeaderLabels( m_ColumnHeaders );
	m_DownloadView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_DownloadView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_DownloadView->setAlternatingRowColors( true );
	m_DownloadView->setRootIsDecorated( false );	
	
	m_MenuBar = new QMenuBar(this);
	/*
	 * File menu
	 */
	m_FileMenu = new QMenu( tr("&File"), m_MenuBar);
	m_File_NewAction = new QAction(tr( "&New" ), m_FileMenu) ;
	m_File_SendToTrayAction = new QAction(tr( "Send to &Tray" ), m_FileMenu) ;
	m_File_ExitAction = new QAction(tr( "E&xit" ), m_FileMenu) ;
	/*
	 * Edit menu 
	 */
	m_EditMenu = new QMenu( tr("&Edit"), m_MenuBar );
	m_Edit_StopAction = new QAction( tr("Stop selected "), m_EditMenu );
	m_Edit_DeleteAction = new QAction( tr("Delete selected "), m_EditMenu );
	m_Edit_RedownloadAction = new QAction( tr("Redownload selected"), m_EditMenu );
	m_Edit_ResumeAction = new QAction( tr ("Resume selected"), m_EditMenu );
	m_Edit_MoveUpAction = new QAction( tr ("Move up "), m_EditMenu );
	m_Edit_MoveDownAction = new QAction( tr ("Move down "), m_EditMenu );

	/*
	 * Settings menu 
	 */
	m_SettingsMenu = new QMenu(tr("&Settings"), m_MenuBar);
	m_Settings_Configure = new QAction(tr("Configure"), m_SettingsMenu);
	m_Settings_Configure->setIcon( QIcon(":/configure.png") );
	/*
	 * about menu
	 */
	m_AboutMenu = new QMenu( tr("&About"), m_MenuBar);
	m_AboutQtAction = new QAction(tr("About Qt"), m_AboutMenu );
	m_AboutQRapidshareAction =new QAction(tr("About QRapidshare"), m_AboutMenu ); 
	
	/*
	 * context menu
	 */
	m_qpContextMenu = new QMenu ( tr("Context menu"), this);
	m_qpContextRemoveAction = new QAction(tr("Stop & Remove"), m_qpContextMenu);
	
	InitializeSystemTray();
	ConnectActions();
	SetupUi();
	ReadSettings();
	setWindowTitle( APPLICATION_NAME );
	
};
MainWindow::~MainWindow() throw ()
{
	RSDM_LOG_FUNC ;
	WriteSettings();
	DisConnectActions();

	DeInitialize();
};
unsigned int MainWindow::GetRapidshareDownloadProgressAt(const unsigned int & at ) 
{
	const QRapidshareDownload* pRSDonwload = m_RapidshareDownloadManager->GetAt(at);
	if(pRSDonwload == NULL )
		return -1;
	int iRet = pRSDonwload->GetProgress();
	return iRet;
}
void MainWindow::InitializeSystemTray()
{
	RSDM_LOG_FUNC ;
	if( !QSystemTrayIcon::isSystemTrayAvailable() )
	{
		DebugUtils::q_Warn(tr("No systemtray found") );
		m_apIsSystemTray.reset(new bool(false));
	}
	else
	{
		m_apIsSystemTray.reset(new bool(true));
		m_SystemTrayIcon = new QSystemTrayIcon(this);
		m_SystemTrayMenu = new QMenu(this);
		// actions
		m_STHideAction = new QAction(tr("&Hide"), m_SystemTrayMenu);
		m_STRestoreAction = new QAction(tr("&Restore"), m_SystemTrayMenu);
		m_STQuitAction = new QAction(tr("&Quit"), m_SystemTrayMenu);
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
	
	
}
void MainWindow::ConnectActions()
{
	RSDM_LOG_FUNC ;
	
	// menu actions and slots
	QObject::connect( m_File_ExitAction, SIGNAL(triggered()), this, SLOT(close() ) );
	QObject::connect( m_File_NewAction, SIGNAL(triggered()), this, SLOT(addNewFile()));
	QObject::connect( m_File_SendToTrayAction, SIGNAL(triggered()), this, SLOT(hide()));
	QObject::connect( m_Edit_MoveUpAction, SIGNAL ( triggered()), this, SLOT(Slot_EditMenu_MoveUp()));
	QObject::connect( m_Edit_MoveDownAction, SIGNAL ( triggered()), this, SLOT(Slot_EditMenu_MoveDown()));
	QObject::connect( m_Edit_StopAction, SIGNAL(triggered()), this, SLOT(Slot_EditMenu_Stop()));
	QObject::connect( m_Edit_DeleteAction, SIGNAL(triggered()), this, SLOT(Slot_EditMenu_Delete()));
	QObject::connect( m_Edit_RedownloadAction, SIGNAL(triggered()), this, SLOT(Slot_EditMenu_Redownload()));
	QObject::connect( m_Edit_ResumeAction, SIGNAL(triggered()), this, SLOT(Slot_EditMenu_Resume()));
	QObject::connect( m_Settings_Configure, SIGNAL(triggered()), this, SLOT(showConfigurationDialog()));
	QObject::connect( m_AboutQtAction, SIGNAL(triggered()), this, SLOT(AboutQt()));
	QObject::connect( m_AboutQRapidshareAction, SIGNAL(triggered()), this, SLOT(AboutQR()));
	// system tray actions and slots
	QObject::connect(m_SystemTrayIcon,SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT(Activation( QSystemTrayIcon::ActivationReason )));
	// RapidshareDownloadManager actions and slots
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( DoneDownloading( unsigned int ) ), this, SLOT( DoneDownloading( const unsigned int ) ) );
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( ChangedProgress( unsigned int,unsigned int ) ), 
	this, SLOT( ChangeProgressValue( const unsigned int, const unsigned int  ) ) );
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( ChangedName( unsigned int, QString  ) ), 
	this, SLOT( ChangeProgressName( unsigned int,QString ) ) );
	QObject::connect(m_RapidshareDownloadManager.get(), SIGNAL( DownloadRateChanged( unsigned int , const QString ) ), 
	this, SLOT( DowloadRateChanged( unsigned int , const QString  ) ) );
	
	////////////////////////////////////context menu ///////////////////////////////////////
//	QObject::connect(m_qpContextRemoveAction, SIGNAL(triggered()), this, SLOT());
}
void MainWindow::DisConnectActions() throw ()
{
	m_File_ExitAction->disconnect();
	m_File_NewAction->disconnect();
	m_File_SendToTrayAction->disconnect();
	m_Edit_MoveUpAction->disconnect();
	m_Edit_MoveDownAction->disconnect();
	m_Edit_StopAction->disconnect();
	m_Edit_DeleteAction->disconnect();
	m_Edit_RedownloadAction->disconnect();
	m_Edit_ResumeAction->disconnect();
	m_Settings_Configure->disconnect();
	m_AboutQtAction->disconnect();
	m_AboutQRapidshareAction->disconnect();
	m_SystemTrayIcon->disconnect();
	m_RapidshareDownloadManager.get()->disconnect();
}
void MainWindow::SetupUi()
{
	RSDM_LOG_FUNC ;
	/* 
	 * File Menu
	 */ 
	m_FileMenu->addAction( m_File_NewAction );
	m_FileMenu->addSeparator();
	m_FileMenu->addAction( m_File_SendToTrayAction );
	m_FileMenu->addAction( m_File_ExitAction );
	m_MenuBar->addMenu(m_FileMenu);

	/* 
	* Edit  Menu
	*/
	m_EditMenu->addAction( m_Edit_MoveUpAction );
	m_EditMenu->addAction( m_Edit_MoveDownAction ) ;
	m_EditMenu->addSeparator();
	m_EditMenu->addAction( m_Edit_StopAction);
	m_EditMenu->addAction( m_Edit_DeleteAction);
	m_EditMenu->addAction( m_Edit_RedownloadAction);
	m_EditMenu->addAction( m_Edit_ResumeAction);
	m_MenuBar->addMenu( m_EditMenu);
	/* 
	* Setting  Menu
	*/
	m_SettingsMenu->addAction(m_Settings_Configure);
	m_MenuBar->addMenu(m_SettingsMenu);
	
	/* 
	* About  Menu
	*/
	m_AboutMenu->addAction(m_AboutQtAction);
	m_AboutMenu->addAction(m_AboutQRapidshareAction);
	m_MenuBar->addMenu(m_AboutMenu);

	setCentralWidget( m_DownloadView );
	setMenuBar(m_MenuBar);
	QSize iconSize = this->iconSize();
	iconSize.setWidth(iconSize.width() + 10 );
	iconSize.setHeight(iconSize.height() + 10 );
	setIconSize(iconSize);
	setWindowIcon(QIcon(":/data_transfer.png"));
	
	m_qpContextMenu->addAction(m_qpContextRemoveAction );
}
bool MainWindow::addFileToDownload(const QString & fileToDownload)
{
	RSDM_LOG_FUNC ;
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
			QMessageBox::warning( this, tr( "Error" ),
			tr( "The file %1 cannot not be opened/resumed." ).arg( FileUrl.toString() ) );
		}
		QString dest = dlg->GetDestinationPath();
		baseFileName = QFileInfo(FileUrl.path()).fileName();	
	}
	else
	{
		FileUrl = QUrl(fileToDownload);
		dest = m_DefaultDirPath;
	}
	dest += "/";
	QString fileName = dest + TransformUrlPathToLocalPath( FileUrl.path() );
	QString fileNamePart = fileName + ".part";
	if(QFile(fileNamePart).exists())
	{
		int ret = QMessageBox::information(this,tr("Remove file"),tr("File exist, do you really want to remove file %1").arg( fileNamePart ), 
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if( ret != QMessageBox::Yes ) 
			return false;
	}
	m_RapidshareDownloadManager->AddDownload(FileUrl.toString(), fileName);
	m_DownloadView->AddToDownload( FileUrl.toString(), fileName );
	if( *m_apIsSystemTray )
	{
		m_SystemTrayIcon->showMessage( tr("New download added "), tr("Downloading file %1").arg( FileUrl.toString() ), QSystemTrayIcon::Information);
	}
	return true;
}
/*
 * 
 * SLOTS
 * 
 */
void MainWindow::addNewFile()
{
	RSDM_LOG_FUNC ;
	addFileToDownload();
};
void MainWindow::showConfigurationDialog()
{
	RSDM_LOG_FUNC ;
	//ConfigurationDialog *dialog = new ConfigurationDialog(this);
	Ui_UserSettingsImpl *dialog = new Ui_UserSettingsImpl( m_apSettings.get() );
	dialog->exec();
	if( dialog->result() == QDialog::Accepted )
	{
		qDebug () << "Accepted " ;
		m_DefaultDirPath = dialog->GetDefaultPath();
		SetUser(dialog->GetUser(), dialog->GetPassword());
		int 	maxDownl = dialog->GetMaxDownloadsNumber();
		qDebug() << "maxDownl=" << maxDownl;
		m_RapidshareDownloadManager->SetMaxDownloads(maxDownl);
		WriteSettings();
	}
};
void MainWindow::AboutQR()
{
	RSDM_LOG_FUNC ;
	QMessageBox::aboutQt(this);
}
void MainWindow::AboutQt()
{
	RSDM_LOG_FUNC ;
	QMessageBox::aboutQt(this);
}
void MainWindow::Slot_EditMenu_MoveUp()
{
	RSDM_LOG_FUNC ;
}
void MainWindow::Slot_EditMenu_MoveDown()
{
	RSDM_LOG_FUNC ;

	QList<QTreeWidgetItem *> selected = m_DownloadView->selectedItems();
	if( selected.size() != 1 ) 
		return;
	QTreeWidgetItem * one = selected.at(0);
	QTreeWidgetItem * two = m_DownloadView->itemBelow(one);
	int iPosOne = m_DownloadView->indexOfTopLevelItem( one );
	int iPosTwo = m_DownloadView->indexOfTopLevelItem( two );
	if( one && two )
		m_DownloadView->swap(one,two);
	m_RapidshareDownloadManager->swap(iPosOne,iPosTwo);

	

}
void MainWindow::Slot_EditMenu_Stop()
{
	RSDM_LOG_FUNC ;
	QList<int> iPostoStop = m_DownloadView->StopDownloads(m_DownloadView->selectedItems());
	foreach(int iPos, iPostoStop )
		m_RapidshareDownloadManager->stopAt(iPos);
};
void MainWindow::Slot_EditMenu_Delete()
{
	RSDM_LOG_FUNC ;
	QList<int> iPostoRemove = m_DownloadView->DeleteDownloads(m_DownloadView->selectedItems());
	foreach(int iPos, iPostoRemove )
		m_RapidshareDownloadManager->RemoveAt(iPos) ; 
};
void MainWindow::Slot_EditMenu_Redownload()
{
	;
};
void MainWindow::Slot_EditMenu_Resume()
{
	QList<QTreeWidgetItem *> selected = m_DownloadView->selectedItems();
	foreach( QTreeWidgetItem * pItem, selected ) 
	{
		int iPos = m_DownloadView->indexOfTopLevelItem(pItem);
		QRapidshareDownload* tmp = const_cast<QRapidshareDownload*>( m_RapidshareDownloadManager->GetAt(iPos) ) ;
		if(tmp != NULL)
		{
			if(tmp->GetState() == STOPPED || tmp->GetState() )
				tmp->Download();
		}
	}
};

void MainWindow::keyPressEvent(QKeyEvent *keyPressed)
{
	RSDM_LOG_FUNC ;
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
	if( keyPressed->key() == Qt::Key_Delete )
	{
		QList<QTreeWidgetItem * > selected = m_DownloadView->selectedItems();
		foreach(QTreeWidgetItem *item, selected)
		{
			qDebug() << item;
			//m_RapidsharePoolView.removeAll( item );
			m_DownloadView->removeItemWidget(item,0);
		}
	}
}
void MainWindow::closeEvent(QCloseEvent * event)
{
	RSDM_LOG_FUNC ;
	qDebug() << m_bExit;
	if(! m_bExit )
	{
		event->ignore();
		hide();
	}
}
void MainWindow::hideEvent(QHideEvent *event)
{
	RSDM_LOG_FUNC ;
	SaveUiSettings();
}
void MainWindow::showEvent(QShowEvent *event)
{
	RSDM_LOG_FUNC ;
	LoadUiSettings();
};
void MainWindow::contextMenuEvent(QContextMenuEvent * event)
{
	RSDM_LOG_FUNC ;
	QPoint position = event->pos();
	QPoint globalPos  = event->globalPos();
	QTreeWidgetItem *pItem = m_DownloadView->itemAt( position );
	QTreeWidgetItem *gItem = m_DownloadView->itemAt( globalPos );
	if( NULL == pItem ) 
		return ;
	int iPos = m_DownloadView->indexOfTopLevelItem( pItem );
	m_ContextMenuOnItem = iPos;
	m_qpContextMenu->exec( globalPos );
}
void MainWindow::Activation(QSystemTrayIcon::ActivationReason reason)
{
	RSDM_LOG_FUNC ;
	if(reason != QSystemTrayIcon::Context)
	{
		if(this->isVisible())
			hide();
		else if(this->isHidden())
			showNormal();
	}
};
void MainWindow::ContextMenuRemove()
{
	
}
void MainWindow::close()
{
	RSDM_LOG_FUNC ;
	m_bExit = true; // close chłopaku
	QMainWindow::close();
}

void MainWindow::ChangeProgressName( const unsigned int & at, const QString & name ) 
{
 	RSDM_LOG_FUNC ;
	if( at > (unsigned int)m_DownloadView->topLevelItemCount() )
		return;
	QTreeWidgetItem *hadzia = m_DownloadView->topLevelItem(at);
	if( NULL == hadzia )
		return;
	else
		hadzia->setText(4,name);
}
void MainWindow::ChangeProgressValue(const unsigned int & at ,  const unsigned int & iPerc ) 
{
	RSDM_LOG_FUNC ;
	QTreeWidgetItem *hadzia = m_DownloadView->topLevelItem( at );  
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
	RSDM_LOG_FUNC ;
}
void MainWindow::DowloadRateChanged(const unsigned int & at, const QString & rate)
{
	QTreeWidgetItem *hadzia = m_DownloadView->topLevelItem( at );  
	if( NULL == hadzia ) 
		return;
	else
	{
		hadzia->setText(3, rate);
	}
}
QString MainWindow::TransformUrlPathToLocalPath(const QString & url)
{
	RSDM_LOG_FUNC ;
	QString ret = QString(url);
	ret = ret.remove("/files");
	// remove /213123123/ <- digits only
	ret = ret.remove(QRegExp ("/\\d+/") );
	return ret;
}

void MainWindow::SetUser(const QString & userName, const QString & userPass)
{
	RSDM_LOG_FUNC ;
	
	m_RapidshareDownloadManager->SetUser( QRapidshareUser( userName , userPass ) );
}

void MainWindow::ReadSettings()
{
	RSDM_LOG_FUNC ;
	QString userName = m_apSettings->value( scSettingsPath_UserName ).toString();
	QString userPass = m_apSettings->value( scSettingsPath_UserPass ).toString();
	bool bOk;
	int 	maxDownl = m_apSettings->value( scSettingsPath_MaxDownloads ).toInt(&bOk);
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
	m_DefaultDirPath = m_apSettings->value( scSettingsPath_DefaultPath ).toString();
	if(m_DefaultDirPath.isEmpty())
		m_DefaultDirPath = QDir::homePath();
};
void MainWindow::WriteSettings() throw ()
{
	RSDM_LOG_FUNC ;
	QRapidshareUser rsUser = m_RapidshareDownloadManager->GetUser();
	m_apSettings->setValue( scSettingsPath_UserName ,rsUser.getUserName() );
	m_apSettings->setValue( scSettingsPath_UserPass ,rsUser.getUserPass() );
	SaveUiSettings();
	m_apSettings->setValue( scSettingsPath_MaxDownloads ,m_RapidshareDownloadManager->GetMaxDownloads() );
	m_apSettings->setValue( scSettingsPath_DefaultPath ,m_DefaultDirPath );
	m_Logger << "Syncing settings ";
	m_apSettings->sync();
};
void MainWindow::SaveUiSettings()
{
	RSDM_LOG_FUNC ;
	m_apSettings->setValue(UI_WINDOW_POS, pos());
	m_apSettings->setValue(UI_WINDOW_SIZE, size());
};
void MainWindow::LoadUiSettings()
{
	RSDM_LOG_FUNC ;
	QPoint pos = m_apSettings->value(UI_WINDOW_POS).toPoint();
	move(pos);
	QSize size =  m_apSettings->value(UI_WINDOW_SIZE).toSize();
	resize(size);
};
void MainWindow::DeInitialize() throw () 
{
	RSDM_LOG_FUNC ;
	m_apIsSystemTray.release();
	m_apSettings.release();

	delete m_AboutQRapidshareAction;
	delete m_AboutQtAction;
	delete m_AboutMenu;
	delete m_MenuBar;
	delete m_SystemTrayIcon;
	delete m_SystemTrayMenu;
};
