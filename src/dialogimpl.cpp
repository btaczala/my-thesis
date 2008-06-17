#include "dialogimpl.h"

DialogImpl::DialogImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f), m_RapidShareDownload( "http://rapidshare.com/files/122322166/Me__su____eyrum_vi__spilum_endalaust.part1.rar"  )
{
	setupUi(this);
	m_RapidShareDownload.Download( reinterpret_cast<QObject*>( this ) );
	//cos.Download( reinterpret_cast<QObject*>( this ) );
}
void DialogImpl::requestStarted(const int & idReq)
{
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq << endl;
}
void DialogImpl::requestFinished(const int & idReq, const bool & isFalse)
{	
	qDebug() << __PRETTY_FUNCTION__<< "idReq =  " << idReq << "isFalse=" << isFalse << endl;
	if( isFalse ) 
		qDebug() << m_RapidShareDownload.HttpError() << endl;
}
void DialogImpl::stateChanged(const int & iState)
{	
	qDebug() << __PRETTY_FUNCTION__<< "iState =  " << iState << endl;
}
void DialogImpl::dataSendProgress(const int & done, const int & total)
{	
	qDebug() << __PRETTY_FUNCTION__<< "done =  " << done << "total=" << total << endl;
}
void DialogImpl::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	QT_DEBUG_FUNCTION
	qDebug() << resp.reasonPhrase() << endl;
}
