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

#include <rslogger.h>

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
            this, SLOT(ftpDataTransferProgress(qint64, qint64)));
    connect(m_apFtpObj.get(), SIGNAL(done(bool)), SLOT(ftpDone(bool)));
    connect(m_apFtpObj.get(), SIGNAL( stateChanged(int)), SLOT(ftpStateChanged(int)));
}

void FtpDownload::start()
{
    //fixme:
    //
//    QString tmpName( m_FileDestination.c_str() );
//    tmpName += "/";
//    tmpName += m_FileName.c_str();
//    tmpName += ".part";
//    
//    m_apFile->setFileName( tmpName );
//    qDebug() << tmpName;
//    m_apHost.reset( new QUrl( m_UrlAddress.c_str() ));
    
     if (!m_apHost.get()->isValid() || m_apHost.get()->scheme().toLower() != QLatin1String("ftp"))
        return;
    
    //get from options and set passive connection    m_pDownloadInfo->m_State = DownloadState::INIT;

    setState( DownloadState::INIT, true );
    
    m_apFtpObj->connectToHost(m_apHost.get()->host());
    
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
    if( error )
    {
        qDebug() << "curCmd:"<< m_apFtpObj->currentCommand() << " failed";
        qDebug() << m_apFtpObj->errorString();
       
        if( m_apFtpObj->currentCommand() == QFtp::Cd )
        {
            ftpLogin();
        }
        else
        {
            setState( DownloadState::FAILED, true );
            ftpDisconect();
        }
        return;
    }
    
    switch( m_apFtpObj->currentCommand() )
	{
        case QFtp::ConnectToHost:
        case QFtp::Login:
        {
            makeCdOrDownload();
            break;
        }
        
        case QFtp::Cd:
        {
            qDebug() << "cd";
            beginDownload();
            break;
        }
        case QFtp::Get:
        {
            qDebug() << "download finished";
            ftpDisconect();
        }
        default:
        {
            break;
        }
    }
}

void FtpDownload::ftpCommandStarted ( int id )
{
    qDebug() << "commandStarted";
}

void FtpDownload::ftpDataTransferProgress ( qint64 done, qint64 total )
{
    LOG(QString("dataTransferProgress %1 of %2").arg(done).arg(total));
    emit bytesRead(done,total);
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

void FtpDownload::ftpStateChanged ( int _ftpstate )
{
    qDebug() << "stateChanged";
    if(( state() != DownloadState::DONE ) && ( _ftpstate == QFtp::Unconnected ))
    {
        setState( DownloadState::FAILED, true );
    }
}

void FtpDownload::ftpDisconect()
{
    if( m_apFtpObj.get() )
    {
        qDebug() << "abort and close connection";
        m_apFtpObj->abort();
    }
    m_apFile->close();
}

void FtpDownload::timerEvent(QTimerEvent *event)
{

}

void FtpDownload::renameFile()
{

}

void FtpDownload::beginDownload()
{
    if( m_apFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        //fixme
        
       // setState( DownloadState::DOWNLOADING, true );
//        m_apFtpObj.get()->get(m_FileName.c_str(), m_apFile.get());
    }
    else
    {
       setState(DownloadState::FAILED, true );
        ftpDisconect();
    }
}

void FtpDownload::ftpLogin()
{
    if( m_Options )
    {
        m_user = QString::fromStdString(boost::any_cast<std::string>( m_Options->option( "username" )).c_str());
        m_pass = QString::fromStdString(boost::any_cast<std::string>( m_Options->option( "password" )).c_str());
    }
    
    if( m_user.isEmpty() || m_pass.isEmpty())
    {
        if (!m_apHost.get()->userName().isEmpty())
        {
            m_user = QUrl::fromPercentEncoding( m_apHost.get()->userName().toLatin1());
            m_pass = m_apHost.get()->password();
        }
    }
    if( !m_user.isEmpty() && !m_pass.isEmpty() )
        m_apFtpObj->login( m_user , m_pass );
}

void FtpDownload::makeCdOrDownload()
{
    //download launch 2 because user should be logged in first
    if( state() == DownloadState::DOWNLOADING )
    {
        beginDownload();
        return;
    }
    
    if (!m_apHost.get()->path().isEmpty())
    {
        QString tmp( m_apHost.get()->path());
        tmp = tmp.left( tmp.lastIndexOf("/"));
        m_apFtpObj->cd( tmp );
    }
    else
    {
        beginDownload();
    }
}

