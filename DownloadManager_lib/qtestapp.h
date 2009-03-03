//
// C++ Interface: qtestapp
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "../proxy_lib/src/proxy.h"
#include <QDebug>
#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QString>
#include <idownload.h>
#include <downloadmanager.h>
#include <QMessageBox>
#include <settings.h>
#include "rslogger.h"
class QTestApp : public QMainWindow
{
    Q_OBJECT
    public:
        QTestApp ( QWidget *parent = NULL ) : QMainWindow(parent)
        {
            m_pTreeWidget = new QTreeWidget( this ) ; 
            m_pTreeWidget->setHeaderLabels(QStringList() << "url"<<"dest"<<"perc");
            setCentralWidget ( m_pTreeWidget ) ; 
            m_DownloadManager.init() ; 
            conn();
        }
        ~QTestApp(){}
        void conn () 
        {
            connect ( &m_DownloadManager, SIGNAL(downloadAdded(int)),this, SLOT ( downloadAdded(int)) );
            connect ( &m_DownloadManager, SIGNAL(progressInfoAt(int,ProgressInfo)), this, SLOT(progressInfo(int,ProgressInfo)) );
            connect ( &m_DownloadManager, SIGNAL(statusChanged(int,DownloadState::States)), this, SLOT(statusChaged(int,DownloadState::States)));
        }
        void addDownload( const QString & rapishareURL, const QString & _destination = QDir::homePath() )
        {
            const IDownload *pDownload = m_DownloadManager.addDownload(rapishareURL.toStdString(), _destination.toStdString() );   
            if ( NULL == pDownload ) 
                FATAL("Downloading failed");
        }   
    public slots:
        void downloadAdded( const int position ) 
        {
            RSDM_LOG_FUNC;
            QTreeWidgetItem *pItem = new QTreeWidgetItem( m_pTreeWidget ) ;
            const IDownload *pDownload = m_DownloadManager.downloadAt(position);
            pItem->setText(0,pDownload->urlAddress().c_str());
            pItem->setText(1,pDownload->urlAddress().c_str());
            QString ss = "0/" + QString::number( pDownload->totalBytes() );
            pItem->setText(2,ss);
        }
        void progressInfo( int position, const ProgressInfo & _pgInfo ) 
        {
            RSDM_LOG_FUNC;
            QTreeWidgetItem *pItem = m_pTreeWidget->topLevelItem(position);
            pItem->setText(2, QString::number ( _pgInfo._DownloadedBytes ) + "/" +QString::number(_pgInfo._DownloadedBytes ) ) ;
        }
        void statusChaged( int position, DownloadState::States state) 
        {
            RSDM_LOG_FUNC;
            if ( state == DownloadState::FAILED ) 
            {
                LOG(m_DownloadManager.downloadAt(position)->error().c_str());
                QMessageBox  mbox ; 
                QString end = Proxy::settings()->value(SettingsValNames::scPluginPassword,Settings::PLUGINS,"rapidshare").toString();
                mbox.setText(QString(m_DownloadManager.downloadAt(position)->error().c_str()) + QString("\nPassword: ") + end );
                
                mbox.exec();
            }
        }
    private:
        QTreeWidget * m_pTreeWidget ; 
        DownloadManager m_DownloadManager ; 
};
