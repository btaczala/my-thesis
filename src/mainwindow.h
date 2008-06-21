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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QTreeWidget>
#include <QItemDelegate>
#include <QtGui>
#include <QtGlobal>
#include "Ui_AddDownloadFile.h"
#include "rapidsharemanager/qrapidsharedownload.h"
#include "rapidsharemanager/debugutils.h"

/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/
class DownloadView : public QTreeWidget
{
	Q_OBJECT
public:
	DownloadView(QWidget *parent = 0 ) ;
signals:
	void fileDropped(const QString &fileName);

protected:
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0);
		~MainWindow();
		int GetProgress() const 
		{
			return m_progress;
		}
	protected:
		virtual void keyPressEvent (QKeyEvent *keyEvent);
	private:
		/// methods !!!! 
		void						ConnectActions();
		void 						SetupUi();
		void						ClearPool();
		bool						addFileToDownload(const QString & fileToDownload = QString(""));
		QString						TransformUrlPathToLocalPath(const QString & url); 
		void						SetUser(const QString & userName, const QString & userPass ) ;
		//system tray
		void 						InitializeSystemTray();
		
		
		/// fields !!!
		QStringList					m_ColumnHeaders;
		QPointer<DownloadView>				m_DownloadView;
		QPointer<QMenuBar>				m_MenuBar;
		QPointer<QMenu>					m_FileMenu;
		QPointer<QAction>				m_File_NewAction;
		QPointer<QAction>				m_File_SendToTrayAction;
		QPointer<QAction>				m_File_ExitAction;
		// system tray 
		QPointer<QSystemTrayIcon>			m_SystemTrayIcon;
		QPointer<QMenu>					m_SystemTrayMenu;
		QPointer<QAction>				m_STHideAction;
		QPointer<QAction>				m_STRestoreAction;
		QPointer<QAction>				m_STPauseAction;
		QPointer<QAction>				m_STUnPauseAction;
		QPointer<QAction>				m_STQuitAction;
		std::auto_ptr<bool>				m_apIsPaused;
		std::auto_ptr<bool>				m_apIsSystemTray;
		// view
		int						m_progress;
		QMap<QTreeWidgetItem*,QRapidshareDownload *> 	m_RapidsharePool; 
		std::auto_ptr<QRapidshareUser>			m_apRapidshareUser;
	private slots:
		void						addNewFile();
		void 						ChangeProgressName(const QString & name ) ;
		void 						ChangeProgressValue(const int & iPerc);
		void						close();
		// regarding system tray
		void						reallyQuit();
		
};




#endif
