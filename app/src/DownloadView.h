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
	/**
	 * AddToDownloads
	 * @param urlPath 
	 * @param fileDest 
	 */
	void						AddToDownload(const QString & urlPath, const QString & fileDest);
	/**
	 * DeleteDownloads
	 * @param toDelete 
	 * @return 
	 */
	QList<int>					DeleteDownloads(const QList<QTreeWidgetItem*> & toDelete) ;
	/**
	 * StopDownloads
	 * @param toStop 
	 * @return 
	 */
	QList<int>					StopDownloads(const QList<QTreeWidgetItem *> & toStop ) ;
	/**
	 * swap
	 * @param what 
	 * @param where 
	 */
	void						swap(const QTreeWidgetItem * what, const QTreeWidgetItem * where );
	void						UpdateProgress(const unsigned int & at, const unsigned int & percentage);
	void						UpdateStatus(const unsigned int & at, const QString & status);
private:
	void						ChangeColumn(const unsigned int & at, const unsigned int & columnID, const QString & toChange );
	const static unsigned int 			ProgressColumnID;
	const static unsigned int 			StatusColumnID;
	const static unsigned int 			UrlPathColumnID;
	const static unsigned int 			FilePathColumnID;
	const static unsigned int 			DownloadRateColumnID;
signals:
	void						fileDropped(const QString &fileName);
protected:
	void						dragMoveEvent(QDragMoveEvent *event);
	void						dropEvent(QDropEvent *event);
};
#endif //DOWNLOADVIEW_H