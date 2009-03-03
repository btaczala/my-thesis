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
#ifndef RSLOGGER_H
#define RSLOGGER_H
//
#include <QString>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include <memory>
#include <QHttpRequestHeader>
#include <QMap>
#include "rscommon.h"


#define RSDM_LOG_FUNC LOG("")
#define LOG(a)  qDebug()<< __FILE__ << "@" <<__LINE__ << "in: "<<Q_FUNC_INFO << a
#define PROFILE_FUNCTION ProfileLogger::Log(Q_FUNC_INFO) 
#define LOG_AND_PROFILE PROFILE_FUNCTION ; RSDM_LOG_FUNC ;
#define FATAL(a) qDebug()<< __FILE__ << "@" <<__LINE__ << "in: "<<Q_FUNC_INFO << a 
#define CRITICAL(a) qDebug()<< __FILE__ << "@" <<__LINE__ << "in: "<<Q_FUNC_INFO << a 


class ProfileLogger
{
    public:
        static void Log ( const QString & _funcName) ; 
        static void DumpToQDebug( ) ; 
    private:
        static ProfileLogger * instance() ;
        QMap<QString, int> m_ProfileMap ; 
};

class DebugUtils
{
public:
	static void q_Warn( const QString & toWarn, const char * functionName = NULL );
	static void q_Log ( const QString & toLog, const char * functionName = NULL );
	static void q_Error ( const QString & toLog, const char * functionName = NULL );
	static QString httpReqToString(const QHttpRequestHeader & req);
	static void DumpReponseToFile(const QByteArray & byteArray, const QString & fileName);
};
#endif

