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
#include <QMainWindow>
#include <QDebug>
#include "src/idownload.h"
class QTestApp : public QMainWindow
{
    Q_OBJECT
    public:
        QTestApp ( QWidget *parent = NULL ) : QMainWindow(parent)
        {
        };
        void setConnector( QObject *connector ) 
        {
            connect ( connector,SIGNAL( globalProgress( int ) ),this,SLOT( globalProgress( int ) ));
            connect ( connector,SIGNAL( downloadDoneAt( int ) ),this,SLOT( downloadDoneAt( int ) ));
            connect ( connector,SIGNAL( downloadOnHold( int ) ),this,SLOT( downloadOnHold( int ) ));
            connect ( connector,SIGNAL( statusChanged( int, DownloadState::States  ) ),this,SLOT( statusChanged( int, DownloadState::States  ) ));
            connect ( connector,SIGNAL( bytesReadAt( int, int , int  ) ),this,SLOT( bytesReadAt( int, int , int  ) ) );
        }
    public slots:
        void                    globalProgress(int value)
        {
            qDebug() << value ; 
        };
        void                    statusChanged ( int at, DownloadState::States state  ) 
        {
            qDebug() << at << state ; 
        };
        void                    downloadDoneAt ( int at ) 
        {
            qDebug() << at ; 
        };
        void                    downloadOnHold( int at )
        {
            qDebug() << at ; 
        };
        void                    bytesReadAt( int at, int read, int total )
        {
            qDebug() << at ; 
        };
        
};