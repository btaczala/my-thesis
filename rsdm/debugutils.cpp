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
void DebugUtils::q_Error( const QString & toLog, const char * functionName )
{
	qDebug() << "Error!: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toLog;	 
}

QString DebugUtils::httpReqToString(const QHttpRequestHeader & req)
{
	QString ret;
	ret = "Request Header Debug :\n ";
	foreach(QString tmp, req.keys() )
	{
		ret += tmp;
		ret += ": ";
		ret += req.value(tmp) ;
		ret += " \n ";
	}
	return ret; 
}

void DebugUtils::DumpReponseToFile(const QByteArray & byteArray, const QString & fileName)
{
	QFile file;
	file.setFileName( fileName );
	if( ! file.open( QIODevice::WriteOnly ) )
	{
		DebugUtils::q_Error("Unable to open file",__PRETTY_FUNCTION__);
	}
	if(byteArray.isEmpty())
		DebugUtils::q_Error("Response Error" + fileName,__PRETTY_FUNCTION__);
	file.write( byteArray );
	file.close();
	
}
