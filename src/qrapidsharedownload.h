#ifndef QRAPIDSHAREDOWNLOAD_H
#define QRAPIDSHAREDOWNLOAD_H
//

//



#include <QString>
#include <QHttp>
#include <QHttpRequestHeader>
#include <QDebug>
#include <QObject>

#include "debugutils.h"
class QRapidshareDownload  
{
public:
	QRapidshareDownload(const QString & _UrlFileAddress = QString("") );
	virtual 					~QRapidshareDownload();
	void 						Download(const QObject * toConnect = NULL );
	void						SetUrlFileAddress(const QString & _addr ) ;
	QString						HttpError();
private:

	QString						m_UrlFileAddress; 
	std::auto_ptr<QHttp>		m_apHttpObj;
	std::auto_ptr<QHttpRequestHeader>		m_apHttpRequestHeader;
	
	QString						m_HostName;
	QString						m_PathOnServer;
	QString						m_RequestType;
	void 						ParseAddress();	
	
};
#endif
