#ifndef DIALOGIMPL_H
#define DIALOGIMPL_H
//
#include <QDialog>
#include "ui_dialog.h"
//
#include "qrapidsharedownload.h"
#include "debugutils.h"
class DialogImpl : public QDialog, public Ui::Dialog
{
Q_OBJECT
public:
	DialogImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	
private slots:
public slots:
	void 					requestStarted(const int & idReq) ;
	void 					requestFinished(const int & idReq, const bool & isFalse) ;
	void 					stateChanged(const int & iState) ;
	void 					dataSendProgress(const int & done, const int & total) ;
	void					responseHeaderReceived( const QHttpResponseHeader & resp) ;  
private:
	QRapidshareDownload				m_RapidShareDownload;	
};
#endif


