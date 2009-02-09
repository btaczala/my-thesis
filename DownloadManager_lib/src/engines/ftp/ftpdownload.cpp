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
#include "settings.h"
#include "proxy.h"
#include <rslogger.h>

FtpDownload::FtpDownload( OptionsContainer* options ):IDownload( options )
, m_apFtpObj( new QFtp() )
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
    m_apHost.reset( new QUrl (m_UrlAddress.c_str()) );
    
    if (!m_apHost.get()->isValid() || m_apHost.get()->scheme().toLower() != QLatin1String("ftp"))
    {
        setState(DownloadState::FAILED, true);
        setError("invalid host");
        return;
    }
    
    initFile();
    
    m_FileName = QString::fromStdString(IDownload::m_FileName);
    
    m_path = m_apHost->path();
    m_path = m_path.left(m_path.lastIndexOf("/"));
    m_path.remove(0,1);//'/' remove
    
    //Proxy::settings()->setValue(scPluginUsername, QVariant("tomek"), Settings::PLUGINS, "ftp")
    
    //todo:get from options and set passive connection    m_pDownloadInfo->m_State = DownloadState::INIT;
    
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
       /*
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
        */
    }
    
    switch( m_apFtpObj->currentCommand() )
	{
        case QFtp::ConnectToHost:
        {
            qDebug() << "connected";
            ftpLogin();
        }
        break;
        
        case QFtp::Login:
        {
            qDebug() << "loggedInd";

            QString res = nextDir();
            if( !res.isEmpty())
            {
                m_apFtpObj->cd(res);
            }
            else
                beginDownload();
            
            break;
        }
        
        case QFtp::Cd:
        {
            qDebug() << "cd";
            QString res = nextDir();
            if( !res.isEmpty())
            {
                m_apFtpObj->cd(res);
            }
            else
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

QString FtpDownload::nextDir()
{
    int ind = 0;
    ind = m_path.indexOf("/");
    QString res;
    if( ind != -1 )
    {
        res = m_path.left(ind);
        qDebug() << res;
        m_path = m_path.right(m_path.length() - ind - 1);
        qDebug() << m_path;
    }
    return res;
}

void FtpDownload::ftpDataTransferProgress ( qint64 done, qint64 total )
{
    if( total == -1 )
        qDebug() << m_apFtpObj->errorString();
        
    LOG(QString("dataTransferProgress %1 of %2").arg(done).arg(total));
    emit bytesRead(done,total);
}

void FtpDownload::ftpDone ( bool error )
{
    qDebug() << "done";
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
    closeFile();
}

void FtpDownload::beginDownload()
{
    if( openFile() )
    {
        setState( DownloadState::DOWNLOADING, true );
        qDebug() << m_FileName;
        qDebug() << m_apFile->fileName();
        
        m_apFtpObj.get()->get(m_FileName, m_apFile.get());
    }
    else
    {
        setState(DownloadState::FAILED, true );
        ftpDisconect();
    }
}

void FtpDownload::ftpLogin()
{
    if( Proxy::settings()->value(SettingsValNames::scUseCredentials, Settings::PLUGINS, "ftp").toBool() )
    {
        m_user = Proxy::settings()->value(SettingsValNames::scPluginUsername, Settings::PLUGINS, "ftp").toString();
        m_pass = Proxy::settings()->value(SettingsValNames::scPluginPassword, Settings::PLUGINS, "ftp").toString();
    }
    else if (!m_apHost.get()->userName().isEmpty())
    {
            m_user = QUrl::fromPercentEncoding( m_apHost.get()->userName().toLatin1());
            m_pass = m_apHost.get()->password();
    }
    else
    {
        m_user = "anonymous";
        m_pass = "";
    }
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

