#include "rslogger.h"
//
RSLogger::RSLogger( const QString & fileName) 
{
	m_logFile.setFileName(fileName);
	m_logFile.open(QIODevice::WriteOnly);
}
RSLogger::~RSLogger()
{
	if(m_logFile.isOpen())
		m_logFile.close();
}
void RSLogger::log(const QString & toLog )
{
	
	m_logFile.write(toLog.toStdString().c_str(), toLog.size());
}
//
