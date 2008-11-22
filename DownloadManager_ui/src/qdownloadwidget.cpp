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
#include <QApplication> // for style in QApplication::style()
QDownloadWidget::QDownloadWidget(QWidget * parent) : QTreeWidget(parent )
{
    QStringList headers ;
    headers << tr("#id") << tr("Path") << tr("File size") << tr("Progress") << tr("Download") ; 
    setHeaderLabels(headers);
    setItemDelegateForColumn(0, new DownloadWidgetDelegates::QDownloadIconedItemDelegate(QPixmap(":/download_item.png"), this));
    setItemDelegateForColumn(3, new DownloadWidgetDelegates::QDownloadProgressDelegate(this));
    setItemDelegateForColumn(4, new DownloadWidgetDelegates::QDownloadIconedItemDelegate(QPixmap(":/download_item.png"), this));
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors( true );
    setRootIsDecorated( false );

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, "movie.avi");
    item->setText(4, "Downloading");
    addTopLevelItem(item);
    QTreeWidgetItem* item1 = new QTreeWidgetItem();
    item1->setText(0, "music.mp3");
    item1->setText(4, "Downloading");
    addTopLevelItem(item1);
}
void QDownloadWidget::paintEvent( QPaintEvent *event )
{
	// to add some of specu drawning 
	QTreeWidget::paintEvent(event);
}

namespace DownloadWidgetDelegates
{
    QDownloadIconedItemDelegate::QDownloadIconedItemDelegate(const QPixmap& icon, QObject *parent)
    : QItemDelegate(parent)
    {
        m_icon = icon;
    }

    void QDownloadIconedItemDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
    {
        painter->save();

        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());  
            painter->setPen(option.palette.highlightedText().color());
        }

        QDownloadWidget* p = qobject_cast<QDownloadWidget*>(parent());
        QTreeWidgetItem* item = p->topLevelItem(index.row());

        QRect rect = option.rect;
        rect.setX( rect.x() + horizonatalMargin);
        rect.setY( rect.y() + verticalMargin);

        QIcon ico(m_icon.scaled(16,16));
        ico.paint(painter, rect, Qt::AlignLeft);

        rect.setX( rect.x() + 16 + horizonatalMargin);
        painter->drawText(rect,  item->text(index.column()));

        painter->restore();
    }

    void QDownloadProgressDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
    {
        painter->save();
        QStyleOptionProgressBar progressBarOption;
    	progressBarOption.state = QStyle::State_Enabled;
	    progressBarOption.direction = QApplication::layoutDirection();
	    progressBarOption.rect = option.rect;
	    progressBarOption.fontMetrics = QApplication::fontMetrics();
	    progressBarOption.minimum = 0;
	    progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
	    progressBarOption.textVisible = true;	
	    int iRow = index.row();
	    int progress = 7;// should really be (qobject_cast<QDownloadWidget*>parent())->getProgress();
        // getProgress will return current progress. 
        // m_Progress will be set like:
        /* MainWindows          DownloadManager             IDownload
            |   getProgressAt(int)  |                           |
            |-----------------------|                           |
            |                       | find(int)                 |
            |                       |------                     |
            |                       |     |                     |
            |                       |<-----                     |
            |                       |            getProgress    |
            |                       |--------------------------->
            |                       |<--------------------------|
            |returnProgressAt(int)  |         returnProgress    |
            |-----------------------|                           |
            |                       |                           |

        */
        if(progress == -1 )
		    progress = 0;
        progressBarOption.progress = progress < 0 ? 0 : progress;
	    progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        painter->restore();
    }

}