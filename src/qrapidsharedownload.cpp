#include "qrapidsharedownload.h"

QRapidshareDownload::QRapidshareDownload( const QString & _UrlFileAddress ) : m_UrlFileAddress ( "" ) 
, m_apHttpObj( new QHttp() ), m_apHttpRequestHeader(new QHttpRequestHeader() )
{
	QT_DEBUG_FUNCTION
	SetUrlFileAddress(_UrlFileAddress);
	QObject::connect( m_apHttpObj.get(), SIGNAL( requestStarted( int ) ), this, SLOT( requestStarted( int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( stateChanged( int ) ), this, SLOT( stateChanged( int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( dataSendProgress( int,int ) ), this, SLOT(  dataSendProgress( int,int ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
	QObject::connect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
}
QRapidshareDownload::~QRapidshareDownload()
{
	QT_DEBUG_FUNCTION
	m_apHttpObj.release();
	m_apHttpRequestHeader.release();
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
	QT_DEBUG_FUNCTION
	DebugUtils::q_Log( QString(" _addr=") + _addr)	;
	if( ! _addr.isEmpty() )
	{
		m_UrlFileAddress = _addr;
		ParseAddress();	
	}
}
void QRapidshareDownload::Download(const QString & _addr )
{
	QT_DEBUG_FUNCTION
	DebugUtils::q_Log( QString(" _addr=") + _addr)	;
	SetUrlFileAddress( _addr );
	QT_DEBUG_FUNCTION		
	
	if(m_PathOnServer.isEmpty() || m_HostName.isEmpty() || m_UrlFileAddress.isEmpty() )
	{
		DebugUtils::q_Error( QString(" Unable to translate address"), __PRETTY_FUNCTION__)	;
	}
	m_apHttpRequestHeader->setRequest("GET", m_PathOnServer);
	m_apHttpRequestHeader->setValue("Host", m_HostName);
	m_apHttpObj->setHost(m_HostName);
	m_apHttpObj->request(*( m_apHttpRequestHeader ) );
	m_bIsPrepared = true;
}
QString QRapidshareDownload::Cookie()
{
	return QString("Cookie: user=4625386-%57%4D%32%46%54%5A%67%78%35%59");
}
/********** SLOTS **************/







void QRapidshareDownload::requestStarted(const int & idReq)
{
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq << endl;
}
void QRapidshareDownload::requestFinished(const int & idReq, const bool & isFalse)
{	
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq << "isFalse=" << isFalse << endl;
	if( isFalse )
	{
		qDebug() << m_apHttpObj->errorString() << endl;
		return ; 
	}
	m_apHttpRequestHeader->setRequest("POST",m_PathOnServer);
	m_apHttpRequestHeader->setContentType("application/x-www-form-urlencoded");
	m_apHttpRequestHeader->setValue("Host", m_HostName);
	m_apHttpRequestHeader->setValue("Cookie", Cookie() );
	m_apHttpObj->request(*( m_apHttpRequestHeader ) );	
}
void QRapidshareDownload::stateChanged(const int & iState)
{	
	qDebug() << __PRETTY_FUNCTION__<< "iState =  " << iState << endl;
}
void QRapidshareDownload::dataSendProgress(const int & done, const int & total)
{	
	qDebug() << __PRETTY_FUNCTION__<< "done =  " << done << "total=" << total << endl;
}
void QRapidshareDownload::dataReadProgress(const int & done, const int & total)
{
	qDebug() << __PRETTY_FUNCTION__<< "done =  " << done << "total=" << total << endl;
}
void QRapidshareDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	QT_DEBUG_FUNCTION
	qDebug() << resp.reasonPhrase() << endl;
	int iStatusCode = resp.statusCode(); 
	if( iStatusCode == 200 || iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 303 || iStatusCode == 307 )
	{
		;
	}
	else
		;
}
