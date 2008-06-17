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


/*

How send requests to rapidshare server
Send GET 
GET /files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar HTTP/1.1
with:
Host: rapidshare.com\r\n
Connection: Keep-Alive\r\n
Cookie: user=4625386-%57%4D%32%46%54%5A%67%78%35%59\r\n
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50\r\n
Referer: http://rapidshare.com/files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar\r\n 






*/
class QRapidshareDownload : public QObject
{
Q_OBJECT
public:
	QRapidshareDownload()
	{
		;
	}
	QRapidshareDownload(const QString & _UrlFileAddress = QString("") );
	virtual 					~QRapidshareDownload();
	
	void 						Download(const QString & _addr = QString("") );
	void						SetUrlFileAddress(const QString & _addr ) ;
	/// ???? implement or not ? 
	static	void				DownloadFile(const QString & _addr);
	
private:
	QString									m_UrlFileAddress; 
	std::auto_ptr<QHttp>					m_apHttpObj;
	std::auto_ptr<QHttpRequestHeader>		m_apHttpRequestHeader;
	bool									m_bIsPrepared;
	QString									m_HostName;
	QString									m_PathOnServer;
	QString									m_RequestType;	
	void 									ParseAddress();	
	void 									Post();
	QString									Cookie();
private slots:
	void 									requestStarted(const int & idReq) ;
	void 									requestFinished(const int & idReq, const bool & isFalse) ;
	void 									stateChanged(const int & iState) ;
	void 									dataSendProgress(const int & done, const int & total) ;
	void 									dataReadProgress(const int & done, const int & total) ;
	void									responseHeaderReceived( const QHttpResponseHeader & resp) ;  
};
#endif
