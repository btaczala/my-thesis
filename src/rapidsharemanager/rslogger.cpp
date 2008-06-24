#include "rslogger.h"
//
RSLogger::RSLogger( const QString & fileName ) : m_fileMutex( new QMutex() )
{
	if( ! fileName.isEmpty() )
	{
		SetFile( fileName ) ;
	}
}
RSLogger::~RSLogger()
{
	if( m_fileLogger->isOpen() )
	{
		m_fileLogger->close();
	}
}
void RSLogger::Write( const QString & toLog )
{
	m_fileMutex->lock();
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	QString timeString = now.toString( QString( "dd.MM.yyyy - hh:mm:ss.zzz " ) );
	QString logString = timeString + toLog; 
	m_fileLogger->write( logString.toStdString().c_str(), logString.size() );
	m_fileLogger->write("\n");
	m_fileMutex->unlock();
};
void RSLogger::SetFile(const QString & fileName ) 
{
	QDir dir(RS_LOG_PATH);
	if( !dir.exists() )
		dir.mkpath(dir.path());
	QString filePath = dir.path() + "/" + fileName; 
	m_fileLogger.reset( new QFile( filePath ) );
	if(m_fileLogger->open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
			DebugUtils::q_Error("Unable to open log file " + filePath);		
}
