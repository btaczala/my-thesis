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
#include <QApplication>
#include <QtGui>
#include "columnsconfigdialog.h"
#include <sstream>
#include "actions.h"

const QString QDownloadWidget::QDownloadWidgetColumnInfo::settingsName  = "QDownloadWidgetColumnInfo";

QDownloadWidget::QDownloadWidget(QWidget * parent) : QTreeWidget(parent ),m_pContextMenu( new QMenu() ), m_CurrentColumnID(-1)
{
    InitializeColumns();

    setHeader(new QDownloadHeaderView(this));
    
    connect(header(), SIGNAL(contextMenu(QContextMenuEvent*)), this, SLOT(contextMenu(QContextMenuEvent*)));

    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors( true );
    setRootIsDecorated( false );

    QIcon itemIcon(QPixmap(":/download_item.png"));

    QTreeWidgetItem* item = new QTreeWidgetItem(StandardItem);
    item->setText(0, "movie.avi");
    item->setIcon(0, itemIcon);
    item->setText(4, "Downloading");
    item->setData(0, Qt::UserRole, QVariant(false));
    addTopLevelItem(item);

    QTreeWidgetItem* item1 = new QTreeWidgetItem(StandardItem);
    item1->setText(0, "music.mp3");
    item1->setIcon(0, itemIcon);
    item1->setText(4, "Downloading");
    item1->setData(0, Qt::UserRole, QVariant(false));
    addTopLevelItem(item1);

    m_pContextMenu->addAction(Actions::getAction( Actions::scConfigureColumnsActionText ));

    connect(Actions::getAction( Actions::scConfigureColumnsActionText ), SIGNAL(triggered()), this, SLOT(onConfigureColumns()));
    connect ( Actions::getAction( Actions::scHideCurrentColumnText ) , SIGNAL( triggered() ), this, SLOT( columnHide() ) ) ; 
}

QDownloadWidget::~QDownloadWidget()
{
    SaveColumns();

    delete m_downloadItemDelegate;
}
void QDownloadWidget::InitializeColumns()
{
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnId, tr("id"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnPath,tr("Path"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnFileSize,tr("File size"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnProgress,tr("Progress"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnDownload,tr("Download"),true) );

    QStringList headers;

    QSettings setts("Yattaman", "QDownloadManager");

    QString columns = setts.value(QDownloadWidgetColumnInfo::settingsName).value<QString>();

    for(ColumnCollection::iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        headers << i->getName();
        if (!columns.isEmpty())
            i->setVisible(columns.contains(i->getName()));
    }

    setHeaderLabels(headers);
    
    ReloadColumns();

    m_downloadItemDelegate = new DownloadWidgetDelegates::DownloadItemDelegate(this);

    setItemDelegate(m_downloadItemDelegate);
}

void QDownloadWidget::ReloadColumns(bool readSettings)
{
    for(ColumnCollection::iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        setColumnHidden(i->getId(), !(i->isVisible()));

        setColumnWidth(i->getId(), 100);
    }
}

void QDownloadWidget::SaveColumns()
{
    QSettings setts("Yattaman", "QDownloadManager");
    std::stringstream ss;
    for(ColumnCollection::const_iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        if (!ss.str().empty())
            ss << ";";

        if (i->isVisible())
            ss << i->getName().toStdString();
    }

    setts.setValue(QDownloadWidgetColumnInfo::settingsName, QString(ss.str().c_str()));
}

void QDownloadWidget::paintEvent( QPaintEvent *event )
{
	// to add some of specu drawning 
	QTreeWidget::paintEvent(event);
}

void QDownloadWidget::StartPauseSelectedDownload()
{
  
}

void QDownloadWidget::StopSelectedDownload()
{
    ;
}

void QDownloadWidget::RemoveSelectedDownload()
{
    ;
}

void QDownloadWidget::onConfigureColumns()
{
    ColumnsConfigDialog dialog(m_columns, this);

    connect(&dialog, SIGNAL(columnChanged(QDownloadWidget::QDownloadWidgetColumnInfo*)), this, SLOT(columnChanged(QDownloadWidget::QDownloadWidgetColumnInfo*)));

    dialog.exec();
}

void QDownloadWidget::columnChanged(QDownloadWidget::QDownloadWidgetColumnInfo* column)
{
    setColumnHidden(column->getId(), !(column->isVisible()));
}

void QDownloadWidget::columnHide() 
{
    for (QDownloadWidget::ColumnCollection::size_type i = 0 ; i < m_columns.size() ; ++i ) 
    {
        if ( m_columns[i].getId() == m_CurrentColumnID ) 
        {
            m_columns[i].setVisible( false ) ; 
            columnChanged(&m_columns[i]);
        }
    }
}

void QDownloadWidget::contextMenu(QContextMenuEvent * event )
{
    m_pContextMenu->removeAction( Actions::getAction( Actions::scHideCurrentColumnText ) ) ; 
    QHeaderView *head = header();
    QAbstractItemModel *model = head->model();
    m_CurrentColumnID = head->logicalIndexAt(event->pos());
    QVariant w = model->headerData( m_CurrentColumnID , Qt::Horizontal );
    QString txt = w.toString();
    QAction *pAction = Actions::getAction( Actions::scHideCurrentColumnText ) ;
    pAction->setText( Actions::scHideCurrentColumnText.arg(txt));
    m_pContextMenu->addAction( pAction );
    //connect ( pAction, SIGNAL( triggered() ), this, SLOT( columnHide() ) ) ; 
    contextMenuEvent(event);
}

void QDownloadWidget::contextMenuEvent(QContextMenuEvent * event )
{
    //menu.
    m_pContextMenu->popup(mapToGlobal(event->pos()));
    m_pContextMenu->exec();
}

QTestWidget::QTestWidget(QWidget* parent)
    :QWidget(parent)
{
    QLabel* downloadsLabel = new QLabel(tr("Downloads details"));
    downloadsLabel->setMinimumWidth(180);
    QVBoxLayout* downloadsLayout = new QVBoxLayout;
    downloadsLayout->addSpacing(0);
    downloadsLayout->addWidget(downloadsLabel);
    downloadsLayout->addStretch(1);

    setLayout(downloadsLayout);
}

namespace DownloadWidgetDelegates
{
    DownloadItemDelegate::DownloadItemDelegate(QObject* parent)
        : QItemDelegate(parent)
    {
        connect(parent, SIGNAL(activated(QModelIndex)), this, SLOT(itemActivated(QModelIndex)));
    }

    void DownloadItemDelegate::itemActivated(const QModelIndex& index)
    {
        QDownloadWidget* p = qobject_cast<QDownloadWidget*>(parent());
        QTreeWidgetItem* item = p->topLevelItem(index.row());

        QDownloadWidget::ItemType type = static_cast<QDownloadWidget::ItemType>(item->data(0, Qt::UserRole).value<int>());
       

        if (type != QDownloadWidget::DetailedItem)
        {
            item->setData(0, Qt::UserRole, QVariant(QDownloadWidget::DetailedItem));

            QTestWidget* test = new QTestWidget;
            test->setParent(qobject_cast<QAbstractItemView *>(parent())->viewport());
            m_details[index.row()] = test;
        }
        else
        {
            item->setData(0, Qt::UserRole, QVariant(QDownloadWidget::StandardItem));
            m_details[index.row()]->hide();
            m_details[index.row()]->deleteLater();
            m_details.remove(index.row());
        }
    }

    QSize DownloadItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QDownloadWidget* p = qobject_cast<QDownloadWidget*>(parent());
        QTreeWidgetItem* item = p->topLevelItem(index.row());
        
        QDownloadWidget::ItemType type = static_cast<QDownloadWidget::ItemType>(item->data(0, Qt::UserRole).value<int>());

        if (type == QDownloadWidget::DetailedItem)
        {
            return QSize(option.rect.width(), 120);
        }
        
        return QItemDelegate::sizeHint(option, index);
    }

    void DownloadItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QDownloadWidget* p = qobject_cast<QDownloadWidget*>(parent());
        QTreeWidgetItem* item = p->topLevelItem(index.row());
        
        QDownloadWidget::ItemType type = static_cast<QDownloadWidget::ItemType>(item->data(0, Qt::UserRole).value<int>());

        if (type == QDownloadWidget::DetailedItem)
        {
            drawDetailedItem(painter, option, index);
            return;
        }

        drawStandardItem(painter, option, index);
    }

    void DownloadItemDelegate::drawStandardItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize size = QItemDelegate::sizeHint(option, index);
        
        QStyleOptionViewItem opt(option);
        opt.rect.setHeight(size.height()+2);

        if (index.column() != QDownloadWidget::QDownloadWidgetColumnInfo::ColumnProgress)
        {
            QItemDelegate::paint(painter, opt, index);
            return;
        }
        
        painter->save();
        QStyleOptionProgressBar progressBarOption;
    	progressBarOption.state = QStyle::State_Enabled;
	    progressBarOption.direction = QApplication::layoutDirection();
	    progressBarOption.rect = opt.rect;
	    progressBarOption.fontMetrics = QApplication::fontMetrics();
	    progressBarOption.minimum = 0;
	    progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
	    progressBarOption.textVisible = true;	
	    int iRow = index.row();
	    int progress = 70;// should really be (qobject_cast<QDownloadWidget*>parent())->getProgress();
        // getProgress will return current progress. 
        // m_Progress will be set like:QDownloadWidget
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

    void DownloadItemDelegate::drawDetailedItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize size = QItemDelegate::sizeHint(option, index);
        
        QStyleOptionViewItem opt(option);
        
        if (index.column() == 0)
            opt.rect.setHeight(size.height());
        else
            opt.rect.setHeight(size.height()+2);

        QItemDelegate::paint(painter, opt, index);

        if (index.column() != 0)
        {
            return; 
        }
        
        QWidget* widget = m_details[index.row()];
        
        QRect rect(option.rect);
        rect.setWidth(qobject_cast<QDownloadWidget*>(parent())->width());
        rect.setY(rect.y() + size.height() + 1);
        widget->setGeometry(rect);
        
        widget->show();
    }
}
