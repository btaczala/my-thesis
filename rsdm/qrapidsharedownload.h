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
#ifndef QRAPIDSHAREDOWNLOAD_H
#define QRAPIDSHAREDOWNLOAD_H
// qrapidsharedownload - representing one download from rapidshare server
#include <QString>
#include <QHttp>
#include <QHttpRequestHeader>
#include <QUrl>
#include <QDebug>
#include <QObject>
#include <QFile>
#include "debugutils.h"
#include "qrapidshareuser.h"
#include "rslogger.h"
/*! \enum RapidShareStateMachine
 * \brief hold state machine state. 
 * 
 */
enum RapidShareStateMachine
{
	STOPPED = 0,
	GET_FIRST,
 	GET_SECOND,
  	POST_FIRST,
   	GET_THIRD,
   	DONE,
   	FAILED
};
//! \fn StateToString - translating RapidShareStateMachine into QString 
QString StateToString(const RapidShareStateMachine & rsMachineState);

/*! \struct DownloadInfo
 * \brief Holds information about bytes read. 
 */
class DownloadInfo
{
public:
	DownloadInfo() : m_bytesToRead(0), m_bytesRead(0)
	{
		bytesReadCurrent = 0;
		bytesReadPreviously = 0;

	}
	//! bytesReadCurrent - number of read bytes in current cicle
	int						bytesReadCurrent;
	//! bytesReadPreviously - number of read bytes in previous cicle
	int						bytesReadPreviously;
	int Diff()
	{
		int ret  = bytesReadCurrent - bytesReadPreviously;
		return (ret > 0 ) ? ret : -1 ; 
	}
	unsigned long BytesToRead() const 
	{ 
		return m_bytesToRead; 
	}
	void BytesToRead(const unsigned long & val) 
	{ 
		m_bytesToRead = val; 
	}
	unsigned long BytesRead() const 
	{ 
		return m_bytesRead; 
	}	
	void BytesRead(const unsigned long & val) 
	{ 
		m_bytesRead = val; 
	}
private:
	unsigned long			m_bytesToRead;
	unsigned long			m_bytesRead;
};
/*! \class QRapidshareDownload - representing one download
 * \brief Class for downloading from rapidshare. Set filePath in constructor, or in Download.  
 * 
 */
class QRapidshareDownload : public QObject
{
Q_OBJECT
public:
	// Ctors:
	//! Ctor
	/*!
	 * \brief One, and only one ctor 
	 * \param _UrlFileAddress an QString argument, holding address of file on rapidshare server
	 * \param _fileDest an QString argument, holding destination of local drive
	 */
	QRapidshareDownload(const QString & _UrlFileAddress = QString(""), const QString & _fileDest = QString("") );
	// DCtors:
	//! DCtor
	virtual 							~QRapidshareDownload();
	
	//public methods:
	//!SetUser - set a RSUser
	/*!
	 * \param rsUser as QRapidshareUser. Sets user as rsUser, or if rsUser = EMPTY , does nothing
	 */
	void								SetUser(const QRapidshareUser & rsUser );

	//!SetUser - set a RSUser with given username and pass
	/*!
	 * \param rsName as QString - username
	 * \param pass as QString - user password
	 */
	void								SetUser(const QString& rsName,  const QString& pass);

	//!Download - begin download from RS server
	/*!
	 * \param _addr as QString - same as ctor _UrlFileAddress
	 * \param _fileDest as QString - same as ctor _fileDest
	 */
	void 								Download(const QString & _addr = QString("") , const QString & _fileDest = QString(""));
	void								Restart() ; 
	//!
	/*!
	 * stop - stops download. Download is able to resume
	*/
	void								stop();
	//!
	/*
	 * abort - aborts http request. Download is not able to resume. 
	 *
	 */
	void								abort();
	/*!
	 * SetRapidshareUser - set rapidshare user for download
	 * \param _usr as QRapidskareUser - set rapidshare user for download
	*/
	void								SetRapidshareUser(const QRapidshareUser & _usr ) ;

	/*!
	 * GetState - Return state of download
	 * \return RapidShareStateMachine - state of RapidShareStateMachine
	*/
	RapidShareStateMachine				GetState() const ;
	unsigned int						GetProgress() const ;

	const QString						GetFullUrlFileAddress() const ; 
	const QString						GetFileDestination() const ;
	
private:
	virtual void						timerEvent(QTimerEvent *event);	
	QString								ParseResponseAndGetNewUrl(const QString & resp);
	int									ParseResponseAndGetFileSize(const QString & resp);
	void								TranslateAnswer();
	QString 							ParsePostReponseAndGetAddress(const QString & resp);
	void								SetUrlFileAddress(const QString & _addr ) ;
	
private:
	QString								m_ReferrerFileAddress;
	QString 							m_fileDestination;
	std::auto_ptr<QHttp>				m_apHttpObj;
	std::auto_ptr<QHttpRequestHeader>	m_apHttpRequestHeader;
	std::auto_ptr<QRapidshareUser>		m_apRSUser;
	bool								m_bIsPrepared;
	std::auto_ptr<QUrl>					m_apFileUrl;
	std::auto_ptr<QFile>				m_apFile;
	RapidShareStateMachine				m_RSStateMachine;
	std::auto_ptr<DownloadInfo>			m_downloadInfo;
	QString								m_HostName;
	QString								m_PathOnServer;
	QString								m_RequestType;	
	QFile 								m_FileLog;
	int 								m_timerId;
	qint64								m_readedBytes;
	RSLogger							m_Logger;
	unsigned int						m_Progress;
private slots:
	void 								requestStarted(const int & idReq) ;
	void 								requestFinished(const int & idReq, const bool & isFalse) ;
	void 								stateChanged(const int & iState) ;
	void 								dataSendProgress(const int & done, const int & total) ;
	void 								dataReadProgress(const int & done, const int & total) ;
	void								responseHeaderReceived( const QHttpResponseHeader & resp) ;
	void								done(const bool & error);
	void 								authenticationRequired ( const QString & hostname, quint16 port, QAuthenticator *authenticator );
	void 								proxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );
	void 								readyRead ( const QHttpResponseHeader & resp );
signals:
	void								WhatAmIDoing(const RapidShareStateMachine & what);
	void								DownloadStatus(const int & istate );
	void								Done();
	void								downloadRate(const QString & dwnlRate);
};
#endif
