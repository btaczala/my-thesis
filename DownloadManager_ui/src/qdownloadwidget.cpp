/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza�a                                  *
 *   b@kontrasty.szczecin.pl                                               *
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
#include <QTime>
#include "columnsconfigdialog.h"
#include <sstream>
#include "actions.h"

#include <proxy.h>
#include <settings.h>
#include <downloadmanager.h>

#include <rslogger.h>
#include <downloadmanager.h>

// const QString QDownloadWidget::QDownloadWidgetColumnInfo::settingsName  = "QDownloadWidgetColumnInfo";

QDownloadWidget::QDownloadWidget(QWidget * parent) 
: QTreeWidget(parent )
, m_pContextMenu( new QMenu() )
, m_CurrentColumnID(-1)
{
    InitializeColumns();
    m_pDownloadManager  = new DownloadManager();
    Proxy::setDownloadManager(m_pDownloadManager);
    Proxy::settings()->loadSettings();
    setHeader(new QDownloadHeaderView(this));
    
    connect(header(), SIGNAL(contextMenu(QContextMenuEvent*)), this, SLOT(contextMenu(QContextMenuEvent*)));

    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors( true );
    setRootIsDecorated( false );



    /*
    QTreeWidgetItem* item = new QTreeWidgetItem();

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
    */

    m_pContextMenu->addAction(Actions::getAction( Actions::scConfigureColumnsActionText ));

    connect( Actions::getAction( Actions::scConfigureColumnsActionText ), SIGNAL(triggered()), this, SLOT(onConfigureColumns()));
    connect ( Actions::getAction( Actions::scHideCurrentColumnText ) , SIGNAL( triggered() ), this, SLOT( columnHide() ) ) ; 

    connect( m_pDownloadManager,SIGNAL( globalProgress( int ) ),this,SLOT( globalProgressChanged( int ) ) );
    connect( m_pDownloadManager,SIGNAL( statusChanged( int, DownloadState::States ) ),this,SLOT( statusChanged( int, DownloadState::States ) ) );
    connect( m_pDownloadManager,SIGNAL( downloadDoneAt( int ) ),this,SLOT( downloadDoneAt( int ) ) );
    connect( m_pDownloadManager,SIGNAL( downloadOnHold( int ) ),this,SLOT( downloadOnHold( int ) ) );
    //connect( m_pDownloadManager,SIGNAL( bytesReadAt( int,int,int ) ),this,SLOT( bytesReadAt( int,int,int ) ) );
    //connect( m_pDownloadManager,SIGNAL( downloadRateAt( int, const QString & ) ),this,SLOT( downloadRateAt( int,const QString & ) ) );
    //connect( m_pDownloadManager,SIGNAL( elapsedTimeAt( int, unsigned int  ) ),this,SLOT( elapsedTimeAt( int,unsigned int ) ) );
    connect(m_pDownloadManager,SIGNAL( progressInfoAt( int, const ProgressInfo& ) ),this,SLOT( progressInfoAt( int, const ProgressInfo&  ) ) );
}

QDownloadWidget::~QDownloadWidget()
{
    RSDM_LOG_FUNC;
    SaveColumns();
    disconnect();
    delete m_downloadItemDelegate;
    
    delete m_pDownloadManager;
    m_pDownloadManager = NULL ; 
    Proxy::setDownloadManager(NULL);
}
void QDownloadWidget::InitializeColumns()
{
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnId, tr("id"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnPath,tr("Path"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnFileSize,tr("File size"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnProgress,tr("Progress"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnDownload,tr("Download"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnElapsedTime,tr("Elapsed Time"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnEstimatedTime,tr("Estimated Time"),true) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnDownloadRate,tr("Download Rate"),true) );

    // Proxy::settings()->value(QString, context).value<QString>() 
    QStringList headers;

    //QSettings setts("Yattaman", "QDownloadManager");

    //QString columns = setts.value(QDownloadWidgetColumnInfo::settingsName).value<QString>();
    
    QString columns = Proxy::settings()->value( SettingsValNames::scColumnsInfo, Settings::UI).value<QString>();
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
    //QSettings setts("Yattaman", "QDownloadManager");
    std::stringstream ss;
    for(ColumnCollection::const_iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        if (!ss.str().empty())
            ss << ";";

        if (i->isVisible())
            ss << i->getName().toStdString();
    }
    Proxy::settings()->setValue( SettingsValNames::scColumnsInfo, QString( ss.str().c_str() ), Settings::UI ) ; 
    //setts.setValue(QDownloadWidgetColumnInfo::settingsName, QString(ss.str().c_str()));
}

void QDownloadWidget::paintEvent( QPaintEvent *event )
{
    // to add some of specu drawning 
    QTreeWidget::paintEvent(event);
}

void QDownloadWidget::StartPauseSelectedDownload()
{
    QList<QTreeWidgetItem*> list = selectedItems() ; 
    if ( list.empty() ) 
    {
        LOG(QString(" Unable to get selected downloads" ) );
        return ; 
    };
    int index = -1; 
    Q_FOREACH( QTreeWidgetItem * pItem, list ) 
    {
        
        index = indexOfTopLevelItem(pItem) ;
        if ( index == -1 ) 
            continue ; 
           m_pDownloadManager->startPause( index ) ; 
        
    }
}

void QDownloadWidget::StopSelectedDownload()
{
    QList<QTreeWidgetItem*> list = selectedItems() ;
    if ( list.empty() ) 
        m_pDownloadManager->setState( DownloadManager::STOPPED ) ; 
    int index = -1; 
    Q_FOREACH( QTreeWidgetItem * pItem, list ) 
    {
        index = indexOfTopLevelItem(pItem) ;
        if ( index == -1 ) 
            continue ; 
        m_pDownloadManager->stopDownload( index ) ; 
    }
}

void QDownloadWidget::RemoveSelectedDownload()
{
    QList<QTreeWidgetItem*> list = selectedItems() ;
    int index = -1; 
    Q_FOREACH( QTreeWidgetItem * pItem, list ) 
    {
        index = indexOfTopLevelItem(pItem) ;
        if ( index == -1 ) 
            continue ; 
        m_pDownloadManager->removeDownload( index ) ; 
        delete pItem ; 
        pItem = NULL ; 
    }
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
    contextMenuEvent(event);
}

void QDownloadWidget::contextMenuEvent(QContextMenuEvent * event )
{
    m_pContextMenu->popup(mapToGlobal(event->pos()));
    m_pContextMenu->exec();
}
void QDownloadWidget::addDownload( const QString & url, const QString & fileDestination ) 
{
    
    if ( ! m_pDownloadManager->addDownload(url.toStdString(),fileDestination.toStdString()) ) 
        return ; 
    
    QIcon itemIcon(QPixmap(":/download_item.png"));

    QTreeWidgetItem *pItem = new QTreeWidgetItem(this);
    pItem->setText(0, url);
    pItem->setIcon(0, itemIcon);
    pItem->setText(1, fileDestination);
    pItem->setText(2, QString("0/0"));
    pItem->setText(4, "");
    pItem->setSizeHint(0, QSize(100, 20));
    addTopLevelItem(pItem);
};
void QDownloadWidget::statusChanged( int position, DownloadState::States status )
{
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem ) 
        pItem->setText(4,DownloadStateToString(status));
};
void QDownloadWidget::downloadDoneAt( int position)
{
    QWidget::update();
};
void QDownloadWidget::downloadOnHold( int position)
{
    QWidget::update();
};
void QDownloadWidget::globalProgressChanged( int value)
{
    QWidget::update();
};
/*
void QDownloadWidget::bytesReadAt(int position,int read,int total)
{
    int read_kBytes = read / 1024 ; 
    int total_kBytes = total / 1024 ; 
    emit dataChanged( QModelIndex().child(position,3), QModelIndex().child(position,3) );
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem == NULL ) 
        return ; 
    pItem->setText(2,QString(" %1 / %2 ").arg(read_kBytes).arg(total_kBytes));

    // estimated time ;) 
    bool ok ; 
    double rate = pItem->text(7).toDouble(&ok);
    if ( rate == 0 ) 
        return ; 
    int howMuch = (int)((double)(total_kBytes - read_kBytes) / rate);
    int seconds = howMuch % 60 ; 
    int minutes = (howMuch  - seconds)/60 ; 
    QString sec = (seconds < 10 ? QString::number( 0 ) + QString::number( seconds ) : QString::number( seconds ) ) ; 
    QString text = QString::number( minutes ) + ":" + sec ; 
    pItem->setText(6,text);

}
*/
void QDownloadWidget::progressInfoAt( int position, const ProgressInfo& _info )
{
    int read_kBytes = _info._DownloadedBytes / 1024 ; 
    int total_kBytes = _info._TotalBytes / 1024 ; 
    emit dataChanged( QModelIndex().child(position,3), QModelIndex().child(position,3) );
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem == NULL ) 
        return ; 
    pItem->setText(2,QString(" %1 / %2 ").arg(read_kBytes).arg(total_kBytes));

    // estimated time ;) 
    bool ok ; 
    double rate = _info._DownloadRate.toDouble(&ok);
    if ( !ok ) 
        return ; 

    int howMuch = (int)((double)(total_kBytes - read_kBytes) / rate);
    int seconds = howMuch % 60 ; 
    int minutes = (howMuch  - seconds)/60 ; 
    QString sec = (seconds < 10 ? QString::number( 0 ) + QString::number( seconds ) : QString::number( seconds ) ) ; 
    QString text = QString::number( minutes ) + ":" + sec ; 
    pItem->setText(6,text);
    
    //downloadRate
    pItem->setText(7,_info._DownloadRate);

    //elapsed time
    seconds = _info._ElapsedTime % 60 ; 
    minutes = ( _info._ElapsedTime - seconds)/60 ; 
    
    sec = (seconds < 10 ? QString::number( 0 ) + QString::number( seconds ) : QString::number( seconds ) ) ; 
    text = QString::number( minutes ) + ":" + sec ;  
    pItem->setText(5,text);

}
/*
void QDownloadWidget::downloadRateAt(int position, const QString &downloadRate)
{
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem ) 
        pItem->setText(7,downloadRate);

}
void QDownloadWidget::elapsedTimeAt(int position, unsigned int timeElapsed )
{
    int seconds = timeElapsed % 60 ; 
    int minutes = (timeElapsed - seconds)/60 ; 
    
    QString sec = (seconds < 10 ? QString::number( 0 ) + QString::number( seconds ) : QString::number( seconds ) ) ; 
    QString text = QString::number( minutes ) + ":" + sec ;  
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem ) 
        pItem->setText(5,text);

}
*/
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
    DownloadItemDelegate::DownloadItemDelegate(QObject *parent)
        : QItemDelegate(parent)
    {
        
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
        if( item )
        {
            QDownloadWidget::ItemType type = static_cast<QDownloadWidget::ItemType>(item->data(0, Qt::UserRole).value<int>());
            if (type == QDownloadWidget::DetailedItem)
            {
                return QSize(option.rect.width(), 120);
            } 
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
        int progress = 0 ; 
        QDownloadWidget* a = qobject_cast<QDownloadWidget*>(parent());
         
        IDownload *pDownload = a->m_pDownloadManager->downloadAt( iRow ) ;
        if ( pDownload ) 
        {
            progress = pDownload->progress();
        //fixme! calculate progress here
        }
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



