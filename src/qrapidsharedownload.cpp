#include "qrapidsharedownload.h"

QRapidshareDownload::QRapidshareDownload( const QString & _UrlFileAddress ) : m_UrlFileAddress ( _UrlFileAddress ) 
, m_apHttpObj( new QHttp() ), m_apHttpRequestHeader(new QHttpRequestHeader() )
{
	QT_DEBUG_FUNCTION
	if( !( m_UrlFileAddress.isEmpty() )  )
		ParseAddress() ;
	
}
QRapidshareDownload::~QRapidshareDownload()
{
	QT_DEBUG_FUNCTION
	m_apHttpObj.release();
}

void QRapidshareDownload::ParseAddress()
{
	QT_DEBUG_FUNCTION
	if( (m_UrlFileAddress.indexOf(QString("http://www.rapidshare.com/") ) != - 1) ||
		(m_UrlFileAddress.indexOf(QString("http://rapidshare.com/") ) != - 1) ||
		(m_UrlFileAddress.indexOf(QString("http://rapidshare.com/") ) != - 1) ||
		(m_UrlFileAddress.indexOf(QString("www.rapidshare.com/") ) != - 1) )
	{
		m_HostName = QString("www.rapidshare.com");
		m_PathOnServer = m_UrlFileAddress;
		int z = m_PathOnServer .indexOf("/files");
		m_PathOnServer.replace(0,z,""); 
	}
}
void QRapidshareDownload::SetUrlFileAddress(const QString & _addr )
{
	if( ! _addr.isEmpty() )
	{
		m_UrlFileAddress = _addr;
		ParseAddress();	
	}
}
void QRapidshareDownload::Download( const QObject * toConnect )
{
	QT_DEBUG_FUNCTION
	if( toConnect == NULL ) 
	{
		DebugUtils::q_Warn("No object to connect to!", __PRETTY_FUNCTION__ ); 
	}
	else
	{
		QObject::connect( m_apHttpObj.get(), SIGNAL( requestStarted( int ) ), toConnect, SLOT( requestStarted( int ) ) );
		QObject::connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), toConnect, SLOT( requestFinished( int,bool ) ) );
		QObject::connect( m_apHttpObj.get(), SIGNAL( stateChanged( int ) ), toConnect, SLOT( stateChanged( int ) ) );
		QObject::connect( m_apHttpObj.get(), SIGNAL( dataSendProgress( int,int ) ), toConnect, SLOT(  dataSendProgress( int,int ) ) );
		QObject::connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), toConnect, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );	
	}
	
	m_apHttpRequestHeader->setRequest("GET", m_PathOnServer);
	m_apHttpRequestHeader->setValue("Host", m_HostName);
	m_apHttpObj->setHost(m_HostName);
	m_apHttpObj->request(*( m_apHttpRequestHeader ) );
}
QString QRapidshareDownload::HttpError()
{
	if ( m_apHttpObj->error() == QHttp::NoError ) 
		return QString("No Error ");
	else
		return m_apHttpObj->errorString();
};


