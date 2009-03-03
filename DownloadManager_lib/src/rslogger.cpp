/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała 								   *
 *   b@kontrasty.szczecin.pl   											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "rslogger.h"
//
ProfileLogger* ProfileLogger::instance() {
    static std::auto_ptr<ProfileLogger> spRet ( new ProfileLogger() ) ; 
    return spRet.get() ;
}
void ProfileLogger::Log(const QString& _funcName) {
    ProfileLogger *pLog = instance() ;
    int value = 0 ; 
    if ( pLog->m_ProfileMap.contains(_funcName) ){
        value =pLog->m_ProfileMap[_funcName];
        value++;
    }
    else
        value = 0 ; 
    pLog->m_ProfileMap[_funcName]=value; 
}
void ProfileLogger::DumpToQDebug() {
    ProfileLogger *pLog = instance() ;
    QMap<QString, int>::const_iterator i = pLog->m_ProfileMap.constBegin();
    while (i != pLog->m_ProfileMap.constEnd() ) {
        LOG( "Function :" << i.key() << " was called " << i.value() );
        ++i;
    }
}
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
		DebugUtils::q_Error("Unable to open file",__FUNCTION_NAME__);
	}
	if(byteArray.isEmpty())
		DebugUtils::q_Error("Response Error" + fileName,__FUNCTION_NAME__);
	file.write( byteArray );
	file.close();
	
}

