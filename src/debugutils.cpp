#include "debugutils.h"
//
void DebugUtils::q_Warn( const QString & toWarn, const char * functionName )
{
	qDebug() << "Warning: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toWarn;
		
	 
}
void DebugUtils::q_Log( const QString & toLog, const char * functionName )
{
	qDebug() << "Log: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toLog;	 
}
//
