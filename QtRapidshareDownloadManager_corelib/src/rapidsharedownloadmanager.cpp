#include "rapidsharedownloadmanager.h"

RapidShareDownloadManager::RapidShareDownloadManager( ) : m_Logger("RapidshareDownloadManager.log"), m_iMaxDownload(3), m_iCurrentDownload(0), m_apSettings(new QSettings(scRsdm_SettingsOrganization,scRsdm_SettingsAppName)), m_bStarted(false)
{	
	RSDM_LOG_FUNC ;
	LoadQueue() ; 
}
RapidShareDownloadManager::~RapidShareDownloadManager()
{
	RSDM_LOG_FUNC ;
	SaveQueue();
	foreach(QRapidshareDownload *rsd, m_RapidshareDownloads)
	{
		if(rsd)
		{
			rsd->stop();
			delete rsd;
		}
	}
};
void RapidShareDownloadManager::AddDownload(const QString & toDownload,const QString & where )
{
	RSDM_LOG_FUNC ;
	QMutex mutex;
	QString where2 = where +".part"; 
	QFile whereFile (where2);
	m_Logger.Write("Mutex::Lock");
	mutex.lock();
	m_Logger.Write("Adding download with toDownload: " + toDownload + " and where : " + where  );

	if(FindDownloadWithUrl(toDownload))
	{
		; // so , resume the shit 
		return ; // no need to add and invoke, since it's already added ;)
	}
	if( whereFile.exists() )
	{
		// here decide whether delete file or try to continue downloading. 
		if(whereFile.remove())
			m_Logger << "File " << whereFile.fileName() << "removed"<< RSLogger::rsEndLine;
		else
			m_Logger << "File " << whereFile.fileName() << "not removed"<< RSLogger::rsEndLine;
	}
	QRapidshareDownload *rsd = new QRapidshareDownload( toDownload, where );
	rsd->SetUser( *m_apRapidshareUser );
	QObject::connect( rsd,SIGNAL( WhatAmIDoing(  const RapidShareStateMachine )), this, SLOT( Slot_ChangedState( const RapidShareStateMachine ) ) );
	QObject::connect( rsd,SIGNAL( DownloadStatus( int )), this, SLOT( Slot_ChangeProgressValue( int ) ) );
	QObject::connect( rsd,SIGNAL( Done() ), this, SLOT( Slot_DoneDownloadingOne() ) );
	QObject::connect( rsd,SIGNAL( downloadRate(const QString ) ), this, SLOT( Slot_DownloadRateChanged(const QString) ) );
	m_RapidshareDownloads.push_back(rsd);
	DownloadAsManyAsCan();
	mutex.unlock();
	m_Logger.Write("Mutex::UnLock");
};

void RapidShareDownloadManager::AddDownload( QRapidshareDownload *pDownload )
{
	RSDM_LOG_FUNC ;
	if(pDownload)
	{
		QObject::connect( pDownload,SIGNAL( WhatAmIDoing(  const RapidShareStateMachine )), this, SLOT( Slot_ChangedState( const RapidShareStateMachine ) ) );
		QObject::connect( pDownload,SIGNAL( DownloadStatus( int )), this, SLOT( Slot_ChangeProgressValue( int ) ) );
		QObject::connect( pDownload,SIGNAL( Done() ), this, SLOT( Slot_DoneDownloadingOne() ) );
		QObject::connect( pDownload,SIGNAL( downloadRate(const QString ) ), this, SLOT( Slot_DownloadRateChanged(const QString) ) );
		m_RapidshareDownloads.push_back(pDownload);
		DownloadAsManyAsCan();
	}
};
void RapidShareDownloadManager::SetUser(const QRapidshareUser & user)
{
	RSDM_LOG_FUNC ;
	m_Logger.Write("Setting user: name" + user.getUserName() + " pass:" + user.getUserPass());
	m_apRapidshareUser.reset(new QRapidshareUser( user ) );
	foreach(QRapidshareDownload *pItem, m_RapidshareDownloads )
	{
		pItem->SetUser(*m_apRapidshareUser) ;
	}
};
void RapidShareDownloadManager::SetMaxDownloads(const unsigned int & _MaxDownloads)
{
	RSDM_LOG_FUNC ;
	QString log = QString("Setting max downloads to %1").arg(_MaxDownloads);
	m_Logger.Write(log);
	m_iMaxDownload = _MaxDownloads;
};
const unsigned int RapidShareDownloadManager::GetMaxDownloads()
{
	RSDM_LOG_FUNC ;
	return m_iMaxDownload;
}
void RapidShareDownloadManager::Pause(const QList<int> & listToPause ) 
{
	RSDM_LOG_FUNC ;
	if(listToPause.isEmpty())
	{
		foreach( QRapidshareDownload *rsd, m_RapidshareDownloads)
		{
			rsd->stop();
		}
	}
	else
	{
		foreach(int iter, listToPause)
		{
			QRapidshareDownload *rsd = m_RapidshareDownloads.at( iter );
			rsd->stop();
		}
	}
};
void RapidShareDownloadManager::stopAt(const unsigned int & at )
{
	RSDM_LOG_FUNC ;
	unsigned int iSize =m_RapidshareDownloads.size();
	if( at > (iSize - 1) || iSize == 0 )
		return ;
	QRapidshareDownload * pDownload = m_RapidshareDownloads.at(at);
	if( pDownload )
		pDownload->stop();
};
void RapidShareDownloadManager::resumeAt(const unsigned int & at )
{
	RSDM_LOG_FUNC ;
	unsigned int iSize =m_RapidshareDownloads.size();
	if( at > (iSize - 1) || iSize == 0 )
		return ;
	QRapidshareDownload * pDownload = m_RapidshareDownloads.at(at);
	if( pDownload )
	{
		if( pDownload->GetState() == PAUSED || pDownload->GetProgress() != 0 ) 
			pDownload->Resume();
		else
			pDownload->Download();
	}
};
void RapidShareDownloadManager::stopAll()
{
	foreach(QRapidshareDownload *pDownload, m_RapidshareDownloads )
	{
		pDownload->stop();
	}
};
const QRapidshareDownload * RapidShareDownloadManager::GetAt(const unsigned int & at ) 
{
	unsigned int iSize =m_RapidshareDownloads.size();
	if( at > (iSize -1) || iSize == 0 )
		return NULL;
	const QRapidshareDownload *ret = m_RapidshareDownloads.at(at); 
	return ret;
};
void RapidShareDownloadManager::swap(const unsigned int & one, const unsigned int & two )
{
	if( one > ( unsigned int ) (m_RapidshareDownloads.size() -1)  || two > (unsigned int ) (m_RapidshareDownloads.size() - 1 )  || m_RapidshareDownloads.size() == 0 )
		return;
	QRapidshareDownload * qrsdOne = m_RapidshareDownloads.at( one );
	QRapidshareDownload * qrsdTwo = m_RapidshareDownloads.at( two );
	QRapidshareDownload * temp = qrsdOne ;
	qrsdOne = qrsdTwo;
	qrsdTwo = temp;
}
QRapidshareUser RapidShareDownloadManager::GetUser()
{
	RSDM_LOG_FUNC ;
	if( m_apRapidshareUser.get() )
		return *m_apRapidshareUser;
	else
		return QRapidshareUser("","");
}
void RapidShareDownloadManager::DownloadAsManyAsCan(const unsigned int & startPoint )
{
	RSDM_LOG_FUNC ;
	if(!m_bStarted)
		return ; 
	m_Logger.Write( QString("With startpoint=%1,m_iCurrentDownload=%2, m_iMaxDownload=%3 ").arg(startPoint).arg(m_iCurrentDownload).arg(m_iMaxDownload) );
	if(m_iCurrentDownload == m_iMaxDownload)
		return;
	QRapidshareDownload *rsd;
	int iSize = m_RapidshareDownloads.size(); 
	for(int k = startPoint ; k != m_RapidshareDownloads.size() ; ++k)
	{
		if( m_iCurrentDownload == m_iMaxDownload)
			break;
		rsd = m_RapidshareDownloads.at(k);
		if( rsd )
		{
			if( rsd->GetState() == STOPPED  || rsd->GetState() == GET_FIRST || rsd->GetState() == GET_SECOND || rsd->GetState() == POST_FIRST )
			{
				if(rsd->GetUser() == NULL)
					rsd->SetUser(*m_apRapidshareUser);
				rsd->Download() ; // redownload
				++m_iCurrentDownload ;
			}
			else if ( rsd->GetState() == PAUSED) 
			{
				if( rsd->GetUser() == NULL ) {
					if( m_apRapidshareUser.get () != NULL ) 
						rsd->SetUser(*m_apRapidshareUser) ; 
				}
				if( rsd->GetDownloadHost().isEmpty() )
					rsd->Download();
				else
					rsd->Resume() ; 
			}
		}
	}
	SaveQueue();
}
/*
 *		SLOTS
*/
void RapidShareDownloadManager::Slot_DoneDownloadingOne()
{
	RSDM_LOG_FUNC ;
	QMutex mutex;
	mutex.lock();
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	-- m_iCurrentDownload;
	mutex.unlock();
	emit DoneDownloading(iPos);
	DownloadAsManyAsCan(0);
};
void RapidShareDownloadManager::Slot_ChangedState( const RapidShareStateMachine & state)
{
	RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	UpdateInSettings(iPos,scRsdm_SettingsDownloadListState,StateToString2(state) );

	emit ChangedName( (unsigned int)iPos, StateToString(state) );
};
void RapidShareDownloadManager::Slot_ChangeProgressValue( int progress )
{
	//RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	UpdateInSettings(iPos, scRsdm_SettingsDownloadListPercentage,QString::number(rsd->GetBytesDownloaded()));

	// just for fun 
	UpdateInSettings(iPos,scRsdm_SettingsDownloadListHost,rsd->GetDownloadHost() );
	emit ChangedProgress( iPos, progress );
};
void RapidShareDownloadManager::Slot_DownloadRateChanged(const QString & rate) 
{
	RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	if(rsd)
	{
		int iPos = m_RapidshareDownloads.indexOf(rsd);
		emit DownloadRateChanged( (unsigned int)iPos, rate);
	}
};
void RapidShareDownloadManager::RemoveAt(const unsigned int & iPos)
{
	RSDM_LOG_FUNC ;
	unsigned int iSize = m_RapidshareDownloads.size();
	if( iPos > iSize -1 || iSize == 0)
	{
		m_Logger << "Item not found "<< RSLogger::rsEndLine;
		return;
	}
	QRapidshareDownload* pRSDownload = m_RapidshareDownloads.at(iPos);
	RapidShareStateMachine rssmState = pRSDownload->GetState() ;
	if( rssmState!=STOPPED && rssmState != DONE && rssmState != FAILED )
	{
		pRSDownload->abort();
		--m_iCurrentDownload;
	}
	m_Logger << "Removing item with iPos = " << iPos<< RSLogger::rsEndLine;
	m_RapidshareDownloads.removeAt(iPos);
	delete pRSDownload;
	DownloadAsManyAsCan( 0 );
};
unsigned int RapidShareDownloadManager::size()
{
	RSDM_LOG_FUNC ;
	return m_RapidshareDownloads.size() ;
}

void RapidShareDownloadManager::LoadQueue()
{
	RSDM_LOG_FUNC ;
	QString settingPath ;
	QString settingValue ;
	QString urlPath, fileDest ; 
	bool bOk ; 
	int iSize = m_apSettings->value(scRsdm_SettingsDownloadListSize).toInt(&bOk) ; 
	if ( ! bOk || iSize == 0 ) 
		return ; 
	m_apSettings->setValue(scRsdm_SettingsDownloadListSize, QString::number( iSize ) );
	for ( int j = 0 ; j < iSize ; ++j ) 
	{
		settingPath = scRsdm_SettingsDownloadListUrlPath.arg( j ) ; 
		urlPath = m_apSettings->value(settingPath).toString(); 

		settingPath = scRsdm_SettingsDownloadListFileDest.arg( j ) ; 
		fileDest = m_apSettings->value(settingPath).toString();

		QRapidshareDownload *pDownload = new QRapidshareDownload(urlPath,fileDest);

		settingPath = scRsdm_SettingsDownloadListState.arg( j ) ;
		RapidShareStateMachine state = StringToState(m_apSettings->value(settingPath).toString());
		pDownload->SetState(state) ; 

		settingPath = scRsdm_SettingsDownloadListPercentage.arg( j ) ;
		unsigned int perc = m_apSettings->value(settingPath).toInt(&bOk);
		if ( bOk && perc != 0 )
		{
			pDownload->SetPercentage(perc);
		}

		// host 
		settingPath = scRsdm_SettingsDownloadListHost.arg( j ) ;
		settingValue = m_apSettings->value(settingPath).toString();
		pDownload->SetDownloadHost(settingValue) ;
		
		settingPath = scRsdm_SettingsDownloadListFileSize.arg( j ) ;
		int filesize = m_apSettings->value(settingPath).toInt();
		pDownload->SetFileSize(filesize);
		AddDownload(pDownload) ;
	}
}
void RapidShareDownloadManager::SaveQueue()
{
	RSDM_LOG_FUNC ;
	QString settingPath ;
	QString settingValue ;
	int i = 0;
	int iSize = m_RapidshareDownloads.size(); 
	{
		if ( iSize == 0 ) 
			return ;
		m_apSettings->setValue(scRsdm_SettingsDownloadListSize, QString::number( iSize ) );
		foreach(QRapidshareDownload *pDownload, m_RapidshareDownloads)
		{
			if( pDownload != NULL ) 
			{
				settingPath  = scRsdm_SettingsDownloadListUrlPath.arg( i ) ;
				settingValue = pDownload->GetFullUrlFileAddress() ; 
				m_apSettings->setValue( settingPath, settingValue ) ; 

				settingPath = scRsdm_SettingsDownloadListFileDest.arg( i ) ;
				settingValue = pDownload->GetFileDestination() ; 
				m_apSettings->setValue( settingPath, settingValue ) ; 

				settingPath = scRsdm_SettingsDownloadListState.arg( i ) ;
				settingValue = StateToString2( pDownload->GetState() );
				m_apSettings->setValue( settingPath, settingValue ) ; 

				settingPath = scRsdm_SettingsDownloadListPercentage.arg(i ) ;
				settingValue = QString::number( pDownload->GetBytesDownloaded());
				m_apSettings->setValue( settingPath, settingValue ) ; 

				settingPath = scRsdm_SettingsDownloadListHost.arg(i ) ;
				settingValue = pDownload->GetDownloadHost(); 
				m_apSettings->setValue( settingPath, settingValue ) ; 

				settingPath = scRsdm_SettingsDownloadListFileSize.arg(i ) ;
				settingValue = QString::number( pDownload->GetFileSize() ); 
				m_apSettings->setValue( settingPath, settingValue ) ; 
			}
			++i;
		}
	}
	m_apSettings->sync();
};
bool RapidShareDownloadManager::FindDownloadWithUrl( const QString & findUrl )
{
	foreach(QRapidshareDownload * pDownload, m_RapidshareDownloads)
	{
		if(pDownload)
		{
			if( pDownload->GetFullUrlFileAddress() == findUrl ) 
				return true ; 
		}
	}
	return false ;
}
void RapidShareDownloadManager::UpdateInSettings( const int & position, const QString & what, const QString & value )
{
	RSDM_LOG_FUNC ;
	QString settingIndex  = what.arg( position ) ;
	m_apSettings->setValue( settingIndex,value ) ;
}
const QList<QRapidshareDownload *> RapidShareDownloadManager::GetDownloadList()
{
	RSDM_LOG_FUNC ;
	return m_RapidshareDownloads ; 
}

void RapidShareDownloadManager::Start()
{
	RSDM_LOG_FUNC ;
	m_bStarted = true ; 
}