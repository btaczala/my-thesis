#include "qrapidsharedownload.h"

QString StateToString(const RapidShareStateMachine & rsMachineState)
{
	if(rsMachineState == STOPPED)
		return QString("Not started");
	else if( rsMachineState == GET_FIRST )
		return QString( "Shaking with rapidshare.com" );
	else if( rsMachineState == GET_SECOND )
		return QString( "Requesting second GET" );
	else if( rsMachineState == POST_FIRST )
		return QString( "Requesting first POST" );
	else if( rsMachineState == GET_THIRD )
		return QString( "Downloading data" );
	else 
		return QString( "Downloading finished" );
};
QRapidshareDownload::QRapidshareDownload( const QString & _UrlFileAddress, const QString & _fileDest ) : m_UrlFileAddress ( "" ) 
, m_apHttpObj( new QHttp() ), m_apHttpRequestHeader(new QHttpRequestHeader() ), m_apRSUser(NULL), m_apFileUrl( new QUrl() )
, m_apFile(new QFile() ), m_RSStateMachine( STOPPED ), m_downloadInfo(new DownloadInfo() ), m_timerId(0),m_readedBytes(0)
{
	QT_DEBUG_FUNCTION
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
	QT_DEBUG_FUNCTION
	m_apHttpObj.release();
	m_apHttpRequestHeader.release();
	m_apRSUser.release();
	m_apFileUrl.release() ; 
	m_downloadInfo.release() ;
}
void QRapidshareDownload::SetUrlFileAddress(const QString & _addr )
{
	QT_DEBUG_FUNCTION
	DebugUtils::q_Log( QString(" _addr = ") + _addr)	;
	if( ! _addr.isEmpty() )
	{
		m_apFileUrl.reset( new QUrl( _addr ) );
	}
}
void QRapidshareDownload::Download(const QString & _addr, const QString & _fileDest )
{
	QT_DEBUG_FUNCTION
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
	qDebug() << "First GET";
	qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
	m_apHttpObj->setHost(  m_apFileUrl->host() );
	m_apHttpObj->request( *( m_apHttpRequestHeader ) );
}
/********** SLOTS **************/

void QRapidshareDownload::requestStarted(const int & idReq)
{
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq ;
}
void QRapidshareDownload::requestFinished(const int & idReq, const bool & isFalse)
{	
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq << "isFalse=" << isFalse ;
	m_apFile->close();
	if( isFalse )
	{
		qDebug() << m_apHttpObj->errorString() ;
		return ; 
	}
}
void QRapidshareDownload::stateChanged(const int & iState)
{	
	qDebug() << __PRETTY_FUNCTION__<< "iState =  " << iState ;
}
void QRapidshareDownload::dataSendProgress(const int & done, const int & total)
{	
	qDebug() << __PRETTY_FUNCTION__<< "done =  " << done << "total=" << total ;
}
void QRapidshareDownload::dataReadProgress(const int & done, const int & total)
{
	qDebug() << __PRETTY_FUNCTION__<< "done =  " << done << "total=" << total ;
	if( m_RSStateMachine == GET_THIRD ) 
	{
		double dDone = done;
		double dTotal = total;
		double dResTotal = dDone / dTotal;
		dResTotal *= 100;
		int iTotal = (int)dResTotal;
		qDebug() << "emit :DownloadStatus("<< iTotal <<")";  
		emit DownloadStatus(iTotal) ;
		m_downloadInfo->bytesReadPreviously =m_downloadInfo->bytesReadCurrent;
		m_downloadInfo->bytesReadCurrent = done ; 
		
		qint64 bytes = m_apHttpObj->bytesAvailable();
		m_readedBytes+=bytes;
		char* buff = new char[bytes];
		qint64 iBytes2 = m_apHttpObj->read(buff, bytes);
		if( -1 == iBytes2 )
			DebugUtils::q_Error("ERROR while reading from Http stream ");
		else
		{
			qDebug() << "readed bytes " << iBytes2;
			if( !m_apFile->isOpen())
			{
				qDebug() << ("isClosed, open it");
				if( ! m_apFile->open(QIODevice::WriteOnly | QIODevice::Append) )
				{
					DebugUtils::q_Error("ERROR ..:::DDD124");
					// message = bad
					// 
					return ;
				}
				else
					qDebug() << "opened";
			}
			
			qint64 btmp = m_apFile->write(buff,iBytes2); 
			if( -1 == btmp )
			{
				DebugUtils::q_Error("write failed");
				return;
			}
			else
				qDebug() << ("written successful") << btmp;
		}
		delete[] buff;
	}
}
void QRapidshareDownload::authenticationRequired(const QString & hostname, quint16 port, QAuthenticator * authenticator)
{
	qDebug() << __PRETTY_FUNCTION__<< "hostname =  " << hostname << "port=" << port ;
}
void QRapidshareDownload::proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator)
{
	qDebug() << __PRETTY_FUNCTION__;
}
void QRapidshareDownload::readyRead ( const QHttpResponseHeader & resp )
{
	qDebug() << __PRETTY_FUNCTION__;
}

void QRapidshareDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	QT_DEBUG_FUNCTION
	qDebug() << resp.reasonPhrase() ;
	int iStatusCode = resp.statusCode(); 
	if( iStatusCode == 200 || iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 303 || iStatusCode == 307 )
	{
		;
	}
	else
		qDebug() << "Error response:"<< iStatusCode;
}
void QRapidshareDownload::done(const bool & error)
{
	QT_DEBUG_FUNCTION
	if( error )
	{
		qDebug() << m_apHttpObj->errorString();
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
		qDebug() << "Second GET";		
		qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
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
			DebugUtils::q_Error("jaz pierdole");
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
		qDebug() << "First post"; 		
		qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
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
		qDebug() << "First post"; 
		m_RSStateMachine = GET_THIRD; 
		qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;		
		m_apHttpObj->setHost( m_apFileUrl->host() );
		m_apHttpObj->request( *( m_apHttpRequestHeader ) );
		emit WhatAmIDoing( m_RSStateMachine );
		
		m_timerId = startTimer(1000);
		m_readedBytes = 0;
 	}
 	else if( m_RSStateMachine == GET_THIRD )
 	{
 		
 		emit WhatAmIDoing( m_RSStateMachine );
 		// change fileName without .part
 		m_apFile->close();
 		QFile::rename(m_apFile->fileName(), m_fileDestination);
 		
 		m_RSStateMachine = DONE;
 		emit WhatAmIDoing( m_RSStateMachine );
 		emit Done();
 		
 		killTimer(m_timerId);
	}
	qDebug() << "end of";
};
void QRapidshareDownload::stop()
{
	QT_DEBUG_FUNCTION
	m_apHttpObj->abort();
}
QString QRapidshareDownload::ParseResponseAndGetNewUrl(const QString & resp)
{
	QT_DEBUG_FUNCTION
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("form action");
	if(z < 0 )
	{
		DebugUtils::q_Error("Could not localize substring!");
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
	QT_DEBUG_FUNCTION
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("downloadlink");
	if(z < 0 )
	{
		DebugUtils::q_Error("Hoho, response is zero!");
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
	QT_DEBUG_FUNCTION
	QString line;
	QString newUrl;
	int z = 0;
	z = resp.indexOf("form name");
	if(z < 0 )
	{
		DebugUtils::q_Error("Could not localize substring!");
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
	qDebug() << newUrl;
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
	emit(downloadRate(QString("%1").arg((double) m_readedBytes / 1024),0, 'f',2)); 
	m_readedBytes = 0;
}

