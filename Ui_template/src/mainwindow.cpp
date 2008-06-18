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
#include "downloadviewdelegate.h"
MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent)
{
	m_ColumnHeaders << "File Path" << "Where " << "Progress" << "Download rate " ;
	m_DownloadView.reset( new DownloadView(this) );
	m_DownloadView->setItemDelegate( new DownloadViewDelegate(this) );
	m_DownloadView->setHeaderLabels( m_ColumnHeaders );
	m_DownloadView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_DownloadView->setAlternatingRowColors( true );
	m_DownloadView->setRootIsDecorated( false );
	
	m_MenuBar = new QMenuBar(this);
	m_FileMenu = new QMenu( tr("&File"), m_MenuBar);
	m_File_NewAction = new QAction(tr( "&New" ), m_FileMenu) ;
	m_File_SendToTrayAction = new QAction(tr( "Send to &Tray" ), m_FileMenu) ;
	m_File_ExitAction = new QAction(tr( "E&xit" ), m_FileMenu) ;
	
	ConnectActions();
	SetupUi();
};

MainWindow::~MainWindow()
{
	//m_DownloadView.release();
}
void MainWindow::ConnectActions()
{
	QObject::connect(m_File_ExitAction, SIGNAL(triggered()), this, SLOT(close() ) );
}
void MainWindow::SetupUi()
{
	m_FileMenu->addAction( m_File_NewAction );
	m_FileMenu->addSeparator();
//	m_FileMenu->addAction( m_File_SendToTrayAction );
	m_FileMenu->addAction( m_File_ExitAction );
	m_MenuBar->addMenu(m_FileMenu);
	setCentralWidget( m_DownloadView.get() );
	setMenuBar(m_MenuBar);
}
DownloadView::DownloadView(QWidget * parent) : QTreeWidget( parent ) 
{
	setAcceptDrops(true);
};
void DownloadView::dropEvent(QDropEvent *event)
{
     // Accept drops if the file has a '.torrent' extension and it
     // exists.
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







