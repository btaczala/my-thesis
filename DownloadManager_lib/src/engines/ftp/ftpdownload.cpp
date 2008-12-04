/*
 *  ftpdownload.cpp
 *  qrsdm
 *
 *  Created by Tomasz Dubik on 11/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <boost/any.hpp>
#include <QDebug>

#include "ftpdownload.h"
#include "optionscontainer.h"

FtpDownload::FtpDownload( OptionsContainer* options ):IDownload( options )
, m_apFtpObj( new QFtp() )
, m_apFile( new QFile() )
{
    setConnection();
}

FtpDownload::~FtpDownload()
{
    
}

void FtpDownload::setConnection()
{
    connect(m_apFtpObj.get() , SIGNAL(commandFinished(int, bool)),
            this, SLOT(ftpCommandFinished(int, bool)));     
    connect(m_apFtpObj.get() , SIGNAL(dataTransferProgress(qint64, qint64)),
            this, SLOT(updateDataTransferProgress(qint64, qint64)));
    connect(m_apFtpObj.get(), SIGNAL(done(bool)), SLOT(ftpDone(bool)));
}

void FtpDownload::start()
{
    //m_apFtpObj.reset( new QFtp() );
    //get host
    QString tmpName( m_FileDestination.c_str() );
    tmpName += "/";
    tmpName += m_FileName.c_str();
    tmpName += ".part";
    
    m_apFile->setFileName( tmpName );
    qDebug() << tmpName;
    m_apHost.reset( new QUrl( m_UrlAddress.c_str() ));
    
     if (!m_apHost.get()->isValid() || m_apHost.get()->scheme().toLower() != QLatin1String("ftp"))
        return;
    
    //get from options and set passive connection
    m_apFtpObj->connectToHost(m_apHost.get()->host());
    QString user, pass;
    
    if( m_Options )
    {
        user = QString::fromStdString(boost::any_cast<std::string>( m_Options->option( "username" )).c_str());
        pass = QString::fromStdString(boost::any_cast<std::string>( m_Options->option( "password" )).c_str());
    }
    
    if( user.isEmpty() || pass.isEmpty())
    {
        if (!m_apHost.get()->userName().isEmpty())
        {
            user = QUrl::fromPercentEncoding( m_apHost.get()->userName().toLatin1());
            pass = m_apHost.get()->password();
        }
    }
    
    m_apFtpObj->login( user , pass );
    
    if (!m_apHost.get()->path().isEmpty())
        m_apFtpObj->cd(m_apHost.get()->path());
}

void FtpDownload::stop()
{
    ftpDisconect();
}
 
void FtpDownload::restart()
{
    
}

void FtpDownload::ftpCommandFinished ( int id, bool error )
{
    qDebug() << "commandFinished";
}

void FtpDownload::ftpCommandStarted ( int id )
{
    qDebug() << "commandStarted";
}
void FtpDownload::ftpDataTransferProgress ( qint64 done, qint64 total )
{
    qDebug() << "dataTransferProgress";
}
void FtpDownload::ftpDone ( bool error )
{
    qDebug() << "done";
}
void FtpDownload::ftpListInfo ( const QUrlInfo & i )
{
    qDebug() << "listInfo";
}
void FtpDownload::ftpRawCommandReply ( int replyCode, const QString & detail )
{
    qDebug() << "rawCommandReply";
}
void FtpDownload::ftpReadyRead ()
{
    qDebug() << "readyRead";
}

void FtpDownload::ftpStateChanged ( int state )
{
    qDebug() << "stateChanged";
}

void FtpDownload::ftpDisconect()
{
    if( m_apFtpObj.get() )
    {
        qDebug() << "abort and close connection";
        m_apFtpObj->abort();
        //m_apFtpObj->deleteLater();
        //m_apFtpObj.reset( NULL );
    }
}

void FtpDownload::timerEvent(QTimerEvent *event)
{

}

