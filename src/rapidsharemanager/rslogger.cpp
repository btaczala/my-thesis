#include "rslogger.h"
//
RSLogger::RSLogger( const QString & fileName ) : m_fileMutex( new QMutex() ), m_fileName(NULL), m_fileLogger(NULL)
{
	QString fn = fileName;
	if( ! fileName.isEmpty() )
	{
		if(!fn.endsWith(".log"))
			fn += ".log";
		SetFile( fn ) ;
	}
	m_NumberOfLogs = 0;
}
RSLogger::~RSLogger()
{
	if( m_fileLogger->isOpen() )
	{
		m_fileLogger->close();
	}
	m_fileLogger.release();
	m_fileMutex.release();
	m_fileName.release();
}
bool RSLogger::OpenFile()
{
	if( m_fileName.get() == NULL)
		return false;
	if( m_fileName->isEmpty() ) 
		return false;
	if(m_fileLogger.get() == NULL)
		m_fileLogger.reset(new QFile( *m_fileName ) ) ;
	if(! m_fileLogger->isOpen() )
	{
		if( ! m_fileLogger->open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
		{
			DebugUtils::q_Error("Unable to open log file " + *m_fileName );
			return false ;
		}
		m_fileLogger->write("-----NEW LOG ----- \n");		
	}
	return true;
}
void RSLogger::Write( const QString & toLog )
{
	m_fileMutex->lock();
	if(!OpenFile())
		return;
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	QString timeString = now.toString( QString( "dd.MM.yyyy - hh:mm:ss.zzz " ) );
	QString logString = timeString + toLog; 
	m_fileLogger->write( logString.toStdString().c_str(), logString.size() );
	m_fileLogger->write("\n");
	++m_NumberOfLogs;
	if( m_NumberOfLogs >= MAX_NONFLUSHED_LOGS )
	{
			m_NumberOfLogs = 0;
			m_fileLogger->flush();
	}
	m_fileMutex->unlock();
};
void RSLogger::Error( const QString & toLog )
{
	m_fileMutex->lock();
	if(!OpenFile())
		return;
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	QString timeString = now.toString( QString( "dd.MM.yyyy - hh:mm:ss.zzz " ) );
	QString logString = timeString + "FATAL ERROR! " +  toLog; 
	m_fileLogger->write( logString.toStdString().c_str(), logString.size() );
	m_fileLogger->write("\n");
	++m_NumberOfLogs;
	if( m_NumberOfLogs >= MAX_NONFLUSHED_LOGS )
	{
			m_NumberOfLogs = 0;
			m_fileLogger->flush();
	}
	m_fileMutex->unlock();
};

void RSLogger::SetFile(const QString & fileName ) 
{
	QDir dir(RS_LOG_PATH);
	if( !dir.exists() )
		dir.mkpath(dir.path());
	QString filePath = dir.path() + "/" + fileName;
	m_fileName.reset(new QString( filePath ) ) ;
	
	m_fileLogger.reset( new QFile( filePath ) );
	
}
RSLogger& RSLogger::operator<<(const QString & toLog)
{
	QString log = toLog ; 
	if(log.contains("err:"))
		Error(log.remove("err:"));
	else
		Write(toLog);
	return *this;
}
RSLogger& RSLogger::operator<<(const int & toLog)
{
	Write(QString::number(toLog));
	return *this;
}
