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
		void							ConnectActions();
		void 							SetupUi();
		QStringList						m_ColumnHeaders;
		QPointer<DownloadView>			m_DownloadView;
		QPointer<QMenuBar>				m_MenuBar;
		QPointer<QMenu>					m_FileMenu;
		QPointer<QAction>				m_File_NewAction;
		QPointer<QAction>				m_File_SendToTrayAction;
		QPointer<QAction>				m_File_ExitAction;
		int								m_progress;
		//QRapidshareDownload				m_RapidShareDownload;
		QMap<QTreeWidgetItem*,QRapidshareDownload *> m_RapidsharePool; 
		bool							addFileToDownload(const QString & fileToDownload = QString(""));
	
		QString							TransformUrlPathToLocalPath(const QString & url); 	
	private slots:
		void						addNewFile();
		void 						ChangeProgressName(const QString & name ) ;
		void 						ChangeProgressValue(const int & iPerc);
		
};




#endif
