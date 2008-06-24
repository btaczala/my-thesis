#ifndef RAPIDSHAREDOWNLOADMANAGER_H
#define RAPIDSHAREDOWNLOADMANAGER_H
/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/
#include <QString>
#include <QPointer>
#include <QTimer>
#include "qrapidsharedownload.h"
#include "qrapidshareuser.h"
#include "rslogger.h"
#define RSDM_LOG_FUNC 	m_Logger.Write(__PRETTY_FUNCTION__ ) 
class RapidShareDownloadManager : public QObject{
	Q_OBJECT
public:
	RapidShareDownloadManager();
	~RapidShareDownloadManager();
	void 							AddDownload(const QString & toDownload, const QString & where);
	void							RemoveAt(unsigned int iPos );
	void							SetUser(const QRapidshareUser & user);
	void							SetMaxDownloads(const unsigned int & _MaxDownloads);
	void							Pause(const QList<int> & listToPause = QList<int>() );
	QRapidshareUser					GetUser();
private:
	QList<QRapidshareDownload* >	m_RapidshareDownloads;
	std::auto_ptr<QRapidshareUser>	m_apRapidshareUser;
	unsigned int					m_iMaxDownload;
	unsigned int					m_iCurrentDownload;
	void							DownloadAsManyAsCan(const unsigned int & startPoint = 0);
	QPointer<QTimer>				m_qpDownloadTimer;
	RSLogger						m_Logger;
public slots:
	void							Slot_DoneDownloadingOne();
	void							Slot_ChangedState( const RapidShareStateMachine & );
	void							Slot_ChangeProgressValue( int ); 
private slots:
	void							Slot_Timer();
signals:
	void							DoneDownloading( unsigned int at);
	void							DoneDownloadingAll( );
	void							ChangedProgress(unsigned int at, unsigned int progress ) ;
	void							ChangedName(unsigned int at, QString desc ) ;
	void							Swaped(unsigned int from , unsigned int to ) ; 
};

#endif
