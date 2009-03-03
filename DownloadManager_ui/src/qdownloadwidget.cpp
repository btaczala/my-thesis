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
#include "actions.h"

#include <proxy.h>
#include <settings.h>
#include <downloadmanager.h>
#include <rslogger.h>

const int QDownloadWidget::QDownloadWidgetColumnInfo::VERSION = 1;
const QString QDownloadWidget::ListenerName = QString("QDownloadWidget-Listener") ; 

QDownloadWidget::QDownloadWidget(QWidget * parent) 
: QTreeWidget(parent )
, m_pContextMenu( new QMenu() )
, m_CurrentColumnID(-1)
{
    setHeader(new QDownloadHeaderView(this));
    connect(header(), SIGNAL(sectionMoved(int, int, int)), this, SLOT(sectionMoved(int, int, int)));
    initializeColumns();
    m_pDownloadManager  = Proxy::downloadManager();
    connectWithDownloadManager();
    connect(header(), SIGNAL(contextMenu(QContextMenuEvent*)), this, SLOT(contextMenu(QContextMenuEvent*)));

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setAlternatingRowColors(false);

    m_pContextMenu->addAction(Actions::getAction( Actions::scConfigureColumnsActionText ));

    connect( Actions::getAction( Actions::scConfigureColumnsActionText ), SIGNAL(triggered()), this, SLOT(onConfigureColumns()));
    connect( Actions::getAction( Actions::scHideCurrentColumnText ) , SIGNAL( triggered() ), this, SLOT( columnHide() ) ) ; 
}

QDownloadWidget::~QDownloadWidget()
{
    RSDM_LOG_FUNC;
    disconnectFromDownloadManager();
    saveColumns();
    disconnect();
    delete m_downloadItemDelegate;    
}
void QDownloadWidget::connectWithDownloadManager() {
    connect( m_pDownloadManager, SIGNAL(downloadAdded(int)),this,SLOT(downloadAdded(int)));
    connect( m_pDownloadManager, SIGNAL(downloadRemoved(int)),this,SLOT(downloadRemoved(int)));
    connect( m_pDownloadManager, SIGNAL(progressInfoAt(int,ProgressInfo)),this,SLOT(progressInfoAt(int,ProgressInfo)));
    connect( m_pDownloadManager, SIGNAL(statusChanged(int,DownloadState::States)),this,SLOT(statusChanged(int,DownloadState::States)));
}
void QDownloadWidget::disconnectFromDownloadManager() {
    disconnect( m_pDownloadManager, SIGNAL(downloadAdded(int)),this,SLOT(downloadAdded(int)));
    disconnect( m_pDownloadManager, SIGNAL(downloadRemoved(int)),this,SLOT(downloadRemoved(int)));
    disconnect( m_pDownloadManager, SIGNAL(progressInfoAt(int,ProgressInfo)),this,SLOT(progressInfoAt(int,ProgressInfo)));
    disconnect( m_pDownloadManager, SIGNAL(statusChanged(int,DownloadState::States)),this,SLOT(statusChanged(int,DownloadState::States)));

}
void QDownloadWidget::initializeColumns()
{
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnId, tr("id"), true, 220) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnPath,tr("Path")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnFileSize,tr("File size")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnProgress,tr("Progress")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnDownload,tr("Download")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnElapsedTime,tr("Elapsed Time")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnEstimatedTime,tr("Estimated Time")) );
    m_columns.push_back( QDownloadWidgetColumnInfo( QDownloadWidgetColumnInfo::ColumnDownloadRate,tr("Download Rate")) );

    QStringList headers;
    
    for(ColumnCollection::iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        headers << i->getName();
    }

    setHeaderLabels(headers);
    
    restoreColumns();
    reloadColumns();

    m_downloadItemDelegate = new DownloadWidgetDelegates::DownloadItemDelegate(this);

    setItemDelegate(m_downloadItemDelegate);
}
void QDownloadWidget::reloadColumns(bool readSettings)
{
    for(ColumnCollection::const_iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        header()->resizeSection(i->getId(), i->getWidth());
        
        if (i->getVisualIndex() != UNSET_COL)
            header()->moveSection(i->getId(), i->getVisualIndex());

        setColumnHidden(i->getId(), !(i->isVisible()));
    }
}
void QDownloadWidget::saveColumns()
{    
    for(ColumnCollection::const_iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        QByteArray array;
        QDataStream str(&array, QIODevice::WriteOnly);

        str << QDownloadWidgetColumnInfo::VERSION
            << i->getWidth()
            << i->getVisualIndex()
            << i->isVisible();

        Proxy::settings()->setValue( i->getName(), array, Settings::UI , "columns") ; 
    }
}
void QDownloadWidget::restoreColumns()
{
    for(ColumnCollection::iterator i = m_columns.begin(); i != m_columns.end(); ++i)
    {
        QByteArray array(Proxy::settings()->value( i->getName(), Settings::UI, "columns").toByteArray());

        if (array.isEmpty())
            continue;

        QDataStream str(&array, QIODevice::ReadOnly);

        int version = 0, width = 0, visualIndex;
        bool visible = true;
        str >> version;

        if (version != QDownloadWidgetColumnInfo::VERSION)
            continue;

        str >> width
            >> visualIndex
            >> visible;

        i->setWidth(width);
        i->setVisualIndex(visualIndex);
        i->setVisible(visible);
    }
}
void QDownloadWidget::columnResized(int column, int oldSize, int newSize)
{
    if (m_columns[column].isVisible())
        m_columns[column].setWidth(newSize);
   
    QTreeWidget::columnResized(column, oldSize, newSize);
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
    }

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

    if (list.isEmpty())
        return;

    bool confirmDelete = Proxy::settings()->value(SettingsValNames::scConfirmItemDelete).toBool();

    if (confirmDelete)
    {
        QString msg;

        if (list.count() > 1)
            msg = tr("Do you realy want to delete %1 items?").arg(list.count());
        else
            msg = tr("Do you realy want to delete selected item?");

        QMessageBox dlg(QMessageBox::Question, tr("Confirm"), msg,  QMessageBox::Yes | QMessageBox::No, this);
        dlg.setDefaultButton(QMessageBox::No);

        if (dlg.exec() == QMessageBox::No)
            return;
    }

    Q_FOREACH( QTreeWidgetItem * pItem, list ) 
    {
        index = indexOfTopLevelItem(pItem) ;
        if ( index == -1 ) 
            continue ; 
        m_pDownloadManager->removeDownload( index ) ; 
        //delete pItem ; 
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
void QDownloadWidget::sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex)
{
    m_columns[logicalIndex].setVisualIndex(newVisualIndex);
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
void QDownloadWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
        RemoveSelectedDownload();

    QTreeWidget::keyPressEvent(event);
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
}
void QDownloadWidget::statusChanged( int position, DownloadState::States status )
{
    QTreeWidgetItem *pItem = topLevelItem(position);
    if ( pItem ) 
    {
        pItem->setText(4,status == DownloadState::FAILED ? m_pDownloadManager->downloadAt(position)->error().c_str() : DownloadStateToString(status));
        //pItem->setText(4,DownloadStateToString(status));
    }
}
void QDownloadWidget::downloadDoneAt( int position)
{
    QWidget::update();
}
void QDownloadWidget::downloadOnHold( int position)
{
    QWidget::update();
}
void QDownloadWidget::globalProgressChanged( int value)
{
    QWidget::update();
}
void QDownloadWidget::progressInfoAt( int position, const ProgressInfo& _info )
{
    int read_kBytes = ( _info._DownloadedBytes / 1024 ) ; 
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
void QDownloadWidget::downloadAdded( int newPosition ) 
{
	const IDownload *pDownload = Proxy::downloadManager()->downloadAt(newPosition);
	QFileIconProvider a;
	QString url = pDownload->urlAddress().c_str();
    QString suffix = url.at( url.size() -3 ) ;
    suffix +=url.at( url.size() -2 ) ;
    suffix +=url.at( url.size() -1 ) ;
    QString nonExistingFile = QString( QDir::homePath() +"/"+"temp." + suffix);
    QFile fTmp(nonExistingFile);
    fTmp.open(QIODevice::Append);
    fTmp.close();
    QIcon itemIcon2 = a.icon(QFileInfo(nonExistingFile));
    if ( itemIcon2.isNull() ) 
        itemIcon2 = QIcon(QPixmap(":/download_item.png"));
    QTreeWidgetItem *pItem = new QTreeWidgetItem(this);
    pItem->setText(0, url);
    pItem->setIcon(0, itemIcon2);
	pItem->setText(1, pDownload->destinationAddress().c_str() );
    pItem->setText(2, QString("0/0"));
    pItem->setText(4, "");
    pItem->setSizeHint(0, QSize(100, 20));
    addTopLevelItem(pItem);
    QFile::remove(nonExistingFile);
}
void QDownloadWidget::downloadRemoved(int position)
{
	QTreeWidgetItem *pItem = takeTopLevelItem(position);
	delete pItem;
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
    DownloadItemDelegate::DownloadItemDelegate(QObject *parent)
        : QItemDelegate(parent){}
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
        QStyleOptionViewItem opt(option);

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
         
        const IDownload *pDownload = Proxy::downloadManager()->downloadAt(iRow);//a->m_pDownloadManager->downloadAt( iRow ) ;
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


