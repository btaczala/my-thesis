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
/*! \class RapidShareDownloadManager
	\brief class for downloading from Rapidshare
*/
class RapidShareDownloadManager : public QObject{
	Q_OBJECT
public:
	/*!
	Default ctor
	*/
	RapidShareDownloadManager();
	/*!
	Default dctor. This class should not be derrived ( not a base class ) 
	*/
	~RapidShareDownloadManager();
	/*!
		AddDownload - adds download to queue. If file exist, library removes file, and creates new one. 
		TODO: if toDownload is already in queue, return without adding it to queue
		\param toDownload as QString - url path to file on RS server
		\param where as QString - path on local disk, where the file will be written.  
	*/
	void 							AddDownload(const QString & toDownload, const QString & where);
	/*!
	 *
	 *	RemoveAt - removes download from iPos position
	 *	\param iPos as unsigned int - position of removed item in pool
	*/
	void							RemoveAt(const unsigned int & iPos );
	/*!
		SetUser - sets user for downloading. If user is unset, download will fail. 
		\param user as QRapidshareUser - user
	*/
	void							SetUser(const QRapidshareUser & user);
	/*!
		SetMaxDownloads - set number of max downloads for queue. 
		\param _MaxDownloads as unsigned int - number of maximum parallel downloads. 
	*/
	void							SetMaxDownloads(const unsigned int & _MaxDownloads);
	/*! 
		Return user
		\return QRapidshareUser - *(m_apRapidshareUser.get());
	*/
	QRapidshareUser					GetUser();
	/*! 
		Return number of max parallel downloads
		\return unsigned int - m_iMaxDownload 
	*/
	const unsigned int 				GetMaxDownloads();
	/*! 
		Pause - pauses downloads with id form QList<int> listToPause
		\param listToPause as QList<int> - list of id to dowload 
	*/
	void							Pause(const QList<int> & listToPause = QList<int>() );
	/*!
	* stopAt - stops download at "at" position. Download is not removed from queue, so signals are emited.  
	* \param at as unsigned int - position to be stopped
	*/
	void							stopAt(const unsigned int & at ) ;
	/*!
		Stop All - 
	*/
	void							stopAll();
	/*!
	 *	GetAt - directly get to QRapidshareDownload
	 *	\param pos as unsigned int 
	 */
	const QRapidshareDownload*		GetAt(const unsigned int & pos ) ;
	/*!
	 *	swap - swap downloads
	 *	\param one as unsigned int - position of first element to swap 
	 *	\param two as unsigned int - position of second element to swap 
	 */
	void							swap(const unsigned int & one, const unsigned int & two );
	
	void							SyncQueueWithLocalFile();
private:
	QList<QRapidshareDownload* >	m_RapidshareDownloads;
	std::auto_ptr<QRapidshareUser>	m_apRapidshareUser;
	unsigned int					m_iMaxDownload;
	unsigned int					m_iCurrentDownload;
	RSLogger						m_Logger;
	bool							m_bIsSynced;

	void							SaveStateToFile(const QString & fileName ) ;
	void							LoadStataFromFile(const QString & fileName ) ;

	void							DownloadAsManyAsCan(const unsigned int & startPoint = 0);

public slots:
	void							Slot_DoneDownloadingOne();
	void							Slot_ChangedState( const RapidShareStateMachine & );
	void							Slot_ChangeProgressValue( int );
	void							Slot_DownloadRateChanged(const QString & rate) ;
signals:
	void							DoneDownloading( unsigned int at);
	void							DoneDownloadingAll( );
	void							ChangedProgress(unsigned int at, unsigned int progress ) ;
	void							ChangedName(unsigned int at, QString desc ) ;
	void							Swaped(unsigned int from , unsigned int to ) ;
	void							DownloadRateChanged(unsigned int at, const QString & rate);
};

#endif
