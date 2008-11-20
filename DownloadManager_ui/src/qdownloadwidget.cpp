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
#include "qdownloadwidget.h"
#include <QPaintEvent>
#include <QPainter>
QDownloadWidget::QDownloadWidget(QWidget * parent) : QTreeWidget(parent )
{
	QStringList headers ;
	headers << tr("#id") << tr("Path") << tr("File size") << tr("Progress") << tr("Download") ; 
	setHeaderLabels(headers);
}
void QDownloadWidget::paintEvent( QPaintEvent *event )
{
	// to add some of specu drawning 
	QTreeWidget::paintEvent(event);
}

QDownloadWidgetDelegate::QDownloadWidgetDelegate( QObject *parent ) 
: QItemDelegate(parent)
{
}

void QDownloadWidgetDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	QItemDelegate::paint(painter,option,index) ; 
}