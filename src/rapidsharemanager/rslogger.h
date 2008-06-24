#ifndef RSLOGGER_H
#define RSLOGGER_H
//
#include <QString>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include "debugutils.h"
#include "../common.h"
//
class RSLogger  
{
public:
	RSLogger(const QString & fileName = QString("") );
	~RSLogger();
	void		Write( const QString & toLog ) ;
	void		SetFile(const QString & fileName ) ;
private:
	std::auto_ptr<QFile>	m_fileLogger;
	std::auto_ptr<QMutex>	m_fileMutex;
	
};
#endif
