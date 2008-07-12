/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza�a 								   *
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
 
#ifndef DOWNLOADVIEW_H
#define DOWNLOADVIEW_H
#include <QTreeWidget>
#include <QDropEvent>
#include <QUrl>
#include <QFile>
#include "downloadItem.h"
#include <memory>
class DownloadView : public QTreeWidget 
{
	Q_OBJECT
public:
	DownloadView(QWidget *parent = 0) ;
	~DownloadView() throw ();
	void						AddToDownload(const QString & urlPath, const QString & fileDest);
	QList<int>					DeleteDownloads(const QList<QTreeWidgetItem*> & toDelete) ;
	QList<int>					StopDownloads(const QList<QTreeWidgetItem *> & toStop ) ;
	void						swap(const QTreeWidgetItem * what, const QTreeWidgetItem * where );
private:
	//QList< DownloadItem * >		m_DownloadList;	
signals:
	void						fileDropped(const QString &fileName);
protected:
	void						dragMoveEvent(QDragMoveEvent *event);
	void						dropEvent(QDropEvent *event);

};
#endif //DOWNLOADVIEW_H