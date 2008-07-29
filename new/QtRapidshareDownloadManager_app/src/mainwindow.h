/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała 								   *
 *   b@kontrasty.szczecin.pl   											   *
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QItemDelegate>
#include <QtGui>
#include <QtGlobal>
#include <QSettings>
#include "Ui_AddDownloadFileImpl.h"
#include "common.h"
/// RSDM include files
#include "qrapidsharedownload.h"
#include "rapidsharedownloadmanager.h"
#include "debugutils.h"
#include "rslogger.h"

#include "DownloadView.h"
#include "configurationdialog.h"
#include "Ui_UserSettingsImpl.h"
#include "downloaddelegate.h"

/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/
typedef QMap< QTreeWidgetItem* ,QRapidshareDownload*  > RSPoolType;
class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0);
		~MainWindow() throw(); 
		int GetProgress() const 
		{
			return m_progress;
		}
		unsigned int GetRapidshareDownloadProgressAt(const unsigned int & at ) ;
	protected:
		virtual void keyPressEvent (QKeyEvent *keyEvent);
		virtual void closeEvent(QCloseEvent *event);
		virtual void hideEvent(QHideEvent *event);
		virtual void showEvent(QShowEvent *event);
		virtual void contextMenuEvent(QContextMenuEvent *event);
	private:
		/// methods !!!! 
		void						ConnectActions();
		void						DisConnectActions() throw () ; // cannot throw since dctor calls this function
		// UI
		void 						SetupUi();
		bool						addFileToDownload(const QString & fileToDownload = QString(""));
		bool						addFileToDownload(QRapidshareDownload *pItem ) ;
		QString						TransformUrlPathToLocalPath(const QString & url); 
		void						SetUser(const QString & userName, const QString & userPass ) ;
		//system tray
		void 						InitializeSystemTray();
		// settings
		void						ReadSettings();
		void						WriteSettings() throw(); // cannot throw since dctor calls this function 
		void						SaveUiSettings();
		void						LoadUiSettings();
		// close or just hide
		bool						m_bExit;
		// memory
		void						DeInitialize() throw (); // cannot throw. 
		
		/// fields !!!
		QStringList					m_ColumnHeaders;
		QPointer<DownloadView>		m_DownloadView;
		
		////////////////// menu ///////////////////////////////////
		QPointer<QMenuBar>			m_MenuBar;
		// file menu
		QPointer<QMenu>				m_FileMenu;
		QPointer<QAction>			m_File_NewAction;
		QPointer<QAction>			m_File_SendToTrayAction;
		QPointer<QAction>			m_File_ExitAction;
		// edit menu
		QPointer<QMenu>				m_EditMenu;
		QPointer<QAction>			m_Edit_StopAction;
		QPointer<QAction>			m_Edit_DeleteAction;
		QPointer<QAction>			m_Edit_RedownloadAction;
		QPointer<QAction>			m_Edit_ResumeAction;

		QPointer<QAction>			m_Edit_MoveUpAction;
		QPointer<QAction>			m_Edit_MoveDownAction;
		
		// settings menu
		QPointer<QMenu>				m_SettingsMenu;
		QPointer<QAction>			m_Settings_Configure;
		// about menu
		QPointer<QMenu>				m_AboutMenu;
		QPointer<QAction>			m_AboutQtAction;
		QPointer<QAction>			m_AboutQRapidshareAction;
		
		////////////////// context menu ///////////////////////////////////
		QPointer<QMenu>				m_qpContextMenu;
		QPointer<QAction>			m_qpContextRemoveAction;
		int							m_ContextMenuOnItem;
		
		
		// system tray 
		QPointer<QSystemTrayIcon>	m_SystemTrayIcon;
		QPointer<QMenu>				m_SystemTrayMenu;
		QPointer<QAction>			m_STHideAction;
		QPointer<QAction>			m_STRestoreAction;
		QPointer<QAction>			m_STPauseAction;
		QPointer<QAction>			m_STUnPauseAction;
		QPointer<QAction>			m_STQuitAction;
		std::auto_ptr<bool>			m_apIsSystemTray;
		// view
		int							m_progress;
		//QList<QTreeWidgetItem*>		m_RapidsharePoolView;
		std::auto_ptr<RapidShareDownloadManager> m_RapidshareDownloadManager;
		QPointer<QShortcut>			m_DeleteShortcut;
		
		// settings
		std::auto_ptr<QSettings>	m_apSettings;
		QString						m_DefaultDirPath;
		RSLogger					m_Logger;


		//sync with file 
		QPointer< QTimer >			m_qpSyncTimer ; 
		std::auto_ptr< QString >	m_apSyncFileName ; 

		void						SaveQueue() throw() ; 
		void						LoadQueue() throw() ;
		
	private slots:
		// menu
		void						addNewFile();
		void						showConfigurationDialog();
		void						AboutQR();
		void						AboutQt();

		void						Slot_EditMenu_MoveUp();
		void						Slot_EditMenu_MoveDown();
		void						Slot_EditMenu_Stop();
		void						Slot_EditMenu_Delete();
		void						Slot_EditMenu_Redownload();
		void						Slot_EditMenu_Resume();
		// items 
		void 						ChangeProgressName(const unsigned int & at, const QString & name ) ;
		void 						ChangeProgressValue(const unsigned int & at,  const unsigned int & iPerc);
		void						DoneDownloading( const unsigned int & at );
		void 						DowloadRateChanged(const unsigned int & at, const QString & rate);
		// application
		void						close();
		// view
		//void						DeleteKeyPressed();
		// regarding system tray
		void 						Activation( QSystemTrayIcon::ActivationReason reason);
		/////////////////////////context menu ///////////////////////////////
		void						ContextMenuRemove();

		// sync with file 
		void						Slot_SycnFileTimer() ; 
};
#endif



