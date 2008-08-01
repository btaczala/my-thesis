/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza³a 								   *
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
#ifndef QDOWNLOADWIDGET_H
#define QDOWNLOADWIDGET_H
#include < QTreeWidget >
#include < QItemDelegate >
class QPaintEvent;
class QDownloadWidget : public QTreeWidget
{
	Q_OBJECT
public:
	QDownloadWidget(QWidget * parent = 0) ; 
private:
	QDownloadWidget(const QDownloadWidget & ) ; // hidden 
protected:
	virtual void paintEvent(QPaintEvent *event);
};

class QDownloadWidgetDelegate :	public QItemDelegate
{
	Q_OBJECT
public:
	QDownloadWidgetDelegate(QObject *parent ) ; 
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
};
#endif // QDOWNLOADWIDGET_H