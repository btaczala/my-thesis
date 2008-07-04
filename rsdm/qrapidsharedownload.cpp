#include "qrapidsharedownload.h"

QString StateToString(const RapidShareStateMachine & rsMachineState)
{
	QString toRet = "";
	if(rsMachineState == STOPPED)
		toRet =  QString("Not started");
	else if( rsMachineState == GET_FIRST )
		toRet =  QString( "Shaking with rapidshare.com" );
	else if( rsMachineState == GET_SECOND )
		toRet =  QString( "Requesting second GET" );
	else if( rsMachineState == POST_FIRST )
		toRet =  QString( "Requesting first POST" );
	else if( rsMachineState == GET_THIRD )
		toRet =  QString( "Downloading data" );
	else if( rsMachineState == DONE )
		toRet =  QString( "Downloading finished" );
	else if ( rsMachineState == FAILED )
		toRet =  QString( "Downloading failed!" );
	 return toRet;
};
QRapidshareDownload::QRapidshareDownload( const QString & _UrlFileAddress, const QString & _fileDest ) : m_UrlFileAddress ( "" ) 
, m_apHttpObj( new QHttp() ), m_apHttpRequestHeader(new QHttpRequestHeader() ), m_apRSUser(NULL), m_apFileUrl( new QUrl() )
, m_apFile(new QFile() ), m_RSStateMachine( STOPPED ), m_downloadInfo(new DownloadInfo() ), m_timerId(0),m_readedBytes(0)
, m_Logger(QString("qrapidsharedownload") + QString::number(qrand() ) )

{
	RSDM_LOG_FUNC ;
	SetUrlFileAddress(_UrlFileAddress);
	if( ! _fileDest.isEmpty() )
	{
		m_fileDestination = _fileDest;
		m_apFile->setFileName(m_fileDestination + ".part");
	}
		
	if( ! _UrlFileAddress.isEmpty() )
		m_ReferrerFileAddress = _UrlFileAddress;
	QObject::connect( m_apHttpObj.get(), SIGNAL( requestStarted( int ) ), this, SLOT( requestStarted( int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( stateChanged( int ) ), this, SLOT( stateChanged( int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( dataSendProgress( int,int ) ), this, SLOT(  dataSendProgress( int,int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( authenticationRequired(  const QString , quint16 , QAuthenticator *) ), this, SLOT(  authenticationRequired(  const QString , quint16 , QAuthenticator *)  ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ), this, SLOT(  proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( readyRead ( QHttpResponseHeader ) ), this, SLOT(  readyRead ( QHttpResponseHeader ) ) );	
}
QRapidshareDownload::~QRapidshareDownload()
{
	RSDM_LOG_FUNC ;
	m_apHttpObj.release();
	m_apHttpRequestHeader.release();
	m_apRSUser.release();
	m_apFileUrl.release() ; 
	m_downloadInfo.release() ;
}
void QRapidshareDownload::SetUrlFileAddress(const QString & _addr )
{
	RSDM_LOG_FUNC ;
	DebugUtils::q_Log( QString(" _addr = ") + _addr)	;
	if( ! _addr.isEmpty() )
	{
		m_apFileUrl.reset( new QUrl( _addr ) );
	}
}
void QRapidshareDownload::Download(const QString & _addr, const QString & _fileDest )
{
	RSDM_LOG_FUNC ;
	DebugUtils::q_Log( QString(" _addr=") + _addr)	;
	SetUrlFileAddress( _addr );
	if( !_addr.isEmpty() )
		m_ReferrerFileAddress = _addr;
	if( !_fileDest.isEmpty() )
	{
		m_fileDestination = _fileDest;
		m_apFile->setFileName(m_fileDestination + ".part");
	}
	if( m_ReferrerFileAddress.isEmpty() || m_fileDestination.isEmpty() )
		return ; 
	m_RSStateMachine = GET_FIRST;
	emit WhatAmIDoing( m_RSStateMachine );
	m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
	m_apHttpRequestHeader->setValue("Host",  m_apFileUrl->host() );
	m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
	m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
	m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50");
	m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress );
	m_Logger << "First GET";
	m_Logger << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
	m_apHttpObj->setHost(  m_apFileUrl->host() );
	m_apHttpObj->request( *( m_apHttpRequestHeader ) );
}
/********** SLOTS **************/

void QRapidshareDownload::requestStarted(const int & idReq)
{
	m_Logger << __FUNCTION_NAME__<< "idReq =  " << idReq ;
}
void QRapidshareDownload::requestFinished(const int & idReq, const bool & isFalse)
{	
	m_Logger << __FUNCTION_NAME__<< "idReq =  " << idReq << "isFalse=" << isFalse ;
	m_apFile->close();
	if( isFalse )
	{
		m_Logger << m_apHttpObj->errorString() ;
		return ; 
	}
}
void QRapidshareDownload::stateChanged(const int & iState)
{	
	m_Logger << __FUNCTION_NAME__<< "iState =  " << iState ;
}
void QRapidshareDownload::dataSendProgress(const int & done, const int & total)
{	
	m_Logger << __FUNCTION_NAME__<< "done =  " << done << "total=" << total ;
}
void QRapidshareDownload::dataReadProgress(const int & done, const int & total)
{
	m_Logger << __FUNCTION_NAME__<< "done =  " << done << "total=" << total ;
	if( m_RSStateMachine == GET_THIRD ) 
	{
		double dDone = done;
		double dTotal = total;
		double dResTotal = dDone / dTotal;
		dResTotal *= 100;
		int iTotal = (int)dResTotal;
		m_Logger << "emit :DownloadStatus("<< iTotal <<")";  
		emit DownloadStatus(iTotal) ;
		m_downloadInfo->bytesReadPreviously =m_downloadInfo->bytesReadCurrent;
		m_downloadInfo->bytesReadCurrent = done ; 
		
		qint64 bytes = m_apHttpObj->bytesAvailable();
		m_readedBytes+=bytes;
		char* buff = new char[bytes];
		qint64 iBytes2 = m_apHttpObj->read(buff, bytes);
		if( -1 == iBytes2 )
			m_Logger<<("ERROR while reading from Http stream ");
		else
		{
			m_Logger << "readed bytes " << iBytes2;
			if( !m_apFile->isOpen())
			{
				m_Logger << ("isClosed, open it");
				if( ! m_apFile->open(QIODevice::WriteOnly | QIODevice::Append) ) // TODO: think about it.. 
				{
					m_Logger<<("ERROR ..:::DDD124");
					// message = bad
					// 
					return ;
				}
				else
					m_Logger << "opened";
			}
			
			qint64 btmp = m_apFile->write(buff,iBytes2); 
			if( -1 == btmp )
			{
				m_Logger<<("write failed");
				return;
			}
			else
				m_Logger << ("written successful") << btmp;
		}
		delete[] buff;
	}
}
void QRapidshareDownload::authenticationRequired(const QString & hostname, quint16 port, QAuthenticator * authenticator)
{
	m_Logger << __FUNCTION_NAME__<< "hostname =  " << hostname << "port=" << port ;
}
void QRapidshareDownload::proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator)
{
	m_Logger << __FUNCTION_NAME__;
}
void QRapidshareDownload::readyRead ( const QHttpResponseHeader & resp )
{
	m_Logger << __FUNCTION_NAME__;
}

void QRapidshareDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	RSDM_LOG_FUNC ;
	m_Logger << resp.reasonPhrase() ;
	int iStatusCode = resp.statusCode(); 
	if( iStatusCode == 200 || iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 303 || iStatusCode == 307 )
	{
		;
	}
	else
		m_Logger << "Error response:"<< iStatusCode;
}
void QRapidshareDownload::done(const bool & error)
{
	RSDM_LOG_FUNC ;
	if( error )
	{
		m_Logger << m_apHttpObj->errorString();
		return ;
	}
	if( m_RSStateMachine == GET_FIRST )
	{
		//m_RSStateMachine = GET_SECOND;
		DebugUtils::DumpReponseToFile(m_apHttpObj->readAll(),"get_first");
		m_apHttpRequestHeader->removeValue("Cookie");
		m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
		m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
		m_apHttpRequestHeader->setValue("Accept", "image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, ap//plication/vnd.ms-powerpoint, application/msword, application/x-shockwave-flash, */*" );
		m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress );
		m_Logger << "Second GET";		
		m_Logger << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
		m_apHttpObj->setHost( m_apFileUrl->host() );
		m_apHttpObj->request( *( m_apHttpRequestHeader ));
 		m_RSStateMachine = GET_SECOND ;
	}
	else if( m_RSStateMachine == GET_SECOND )
	{
		QByteArray aa = m_apHttpObj->readAll();
		DebugUtils::DumpReponseToFile(aa,"get_second");
		QString newUrl = aa;
		newUrl = ParseResponseAndGetNewUrl( newUrl );
		if(newUrl.isEmpty() )
		{
			m_Logger<<("Could not find file on server");
			m_RSStateMachine = FAILED;
			emit WhatAmIDoing(m_RSStateMachine);
			emit Done();
			return ;
		}
		SetUrlFileAddress( newUrl );
		emit WhatAmIDoing( m_RSStateMachine );
		m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
		m_apHttpRequestHeader->setRequest("POST", m_apFileUrl->path() );
		m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
		m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
		m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
		m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
		m_apHttpRequestHeader->setValue("Content-Type", "application/x-www-form-urlencoded");
		m_apHttpRequestHeader->setValue("Content-Length", "16");
		m_apHttpRequestHeader->setValue("Referrer", m_ReferrerFileAddress);
		m_Logger << "First post"; 		
		m_Logger << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
		m_apHttpObj->setHost( m_apFileUrl->host() );
		m_apHttpObj->request( *( m_apHttpRequestHeader ), "dl.start=PREMIUM");
 		m_RSStateMachine = POST_FIRST ;
 	}
	else if( m_RSStateMachine == POST_FIRST)
	{
 		QByteArray aa = m_apHttpObj->readAll() ;
 		QString newUrlpath = ParsePostReponseAndGetAddress(QString(aa));
 		DebugUtils::DumpReponseToFile(aa,"post_first");
 		
 		SetUrlFileAddress(newUrlpath);
 		emit WhatAmIDoing( m_RSStateMachine );
 		m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
		m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
		m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
		m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
		m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
		m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
		m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress);
		m_Logger << "First post"; 
		m_RSStateMachine = GET_THIRD; 
		m_Logger << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;		
		m_apHttpObj->setHost( m_apFileUrl->host() );
		m_apHttpObj->request( *( m_apHttpRequestHeader ) );
		emit WhatAmIDoing( m_RSStateMachine );
		
		m_timerId = startTimer(1000);
		m_readedBytes = 0;
 	}
 	else if( m_RSStateMachine == GET_THIRD )
 	{
 		
 		emit WhatAmIDoing( m_RSStateMachine );
 		m_apFile->close();
 		QFile::rename(m_apFile->fileName(), m_fileDestination);
 		
 		m_RSStateMachine = DONE;
 		emit WhatAmIDoing( m_RSStateMachine );
 		emit Done();
 		killTimer(m_timerId);
	}
	m_Logger << "end of";
};
void QRapidshareDownload::stop()
{
	RSDM_LOG_FUNC ;
	m_apHttpObj->abort();
}
QString QRapidshareDownload::ParseResponseAndGetNewUrl(const QString & resp)
{
	RSDM_LOG_FUNC ;
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("form action");
	if(z < 0 )
	{
		m_Logger << "Could not localize substring!" << resp ;
		if( resp.indexOf("The file could not be found") >=0 )
		{
			
		}
		return "";
	}
	for(;;)
	{
		if( resp.at(z) == '\n')
			break;
		line +=resp.at(z++);
	}
	bool quot = false;
	bool write = false;
	for(z = 0;z< line.size();++z)
	{
		if( line.at(z) == '"' )
		{
			if( !quot )
			{
				write = true;
			}
			else
				break;
			quot = !quot;
		}
		if( write )
			newUrl += line.at(z);
	}
	newUrl.remove('"');
	return newUrl;
}
int QRapidshareDownload::ParseResponseAndGetFileSize(const QString & resp)
{
	RSDM_LOG_FUNC ;
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("downloadlink");
	if(z < 0 )
	{
		m_Logger <<"err: Hoho, response is zero!";
		return -1;
	}
	for(;;)
	{
		if( resp.at(z) == '\n')
			break;
		line +=resp.at(z++);
	}
	bool stick = false;
	for(z = 0;z< line.size();++z)
	{
		if( line.at(z) == '|' )
		{
			stick = true;
		}
		if( stick )
		{
			if( line.at(z) == 'K')
				break;
			newUrl += line.at(z);
		}
	}
	newUrl.remove(' ');
	bool ok;
	int ret = newUrl.toInt(&ok);
	if( !ok )
		return -1;
	return ret;
}
QString QRapidshareDownload::ParsePostReponseAndGetAddress( const QString & resp )
{
	RSDM_LOG_FUNC ;
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("form name");
	if(z < 0 )
	{
		m_Logger << " err : Could not localize substring!";
		return "";
	}
	for(;;)
	{
		if( resp.at(z) == '\n')
			break;
		line +=resp.at(z++);
	}
	int iStartPos = line.indexOf("http://");
	if( iStartPos < 0 )
		return "";
	for(int i=iStartPos;i < line.size() ;++i)
	{
		if(line.at(i) =='"')
			break;
		newUrl +=line.at(i);
	}
	m_Logger << newUrl;
	return newUrl;
}
void QRapidshareDownload::SetUser(const QRapidshareUser & rsUser)
{
	m_apRSUser.reset(new QRapidshareUser(rsUser));	
}
void QRapidshareDownload::SetUser(const QString& rsName,  const QString& rsPass)
{
	SetUser(QRapidshareUser(rsName,rsPass));
}

RapidShareStateMachine QRapidshareDownload::GetState()
{
	return m_RSStateMachine;
}

void QRapidshareDownload::timerEvent(QTimerEvent *event)
{
	emit downloadRate( QString("%1").arg( ((double) m_readedBytes / 1024),0, 'f',2) ); 
	m_readedBytes = 0;
}

