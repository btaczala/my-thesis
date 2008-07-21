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
#ifndef RSCOMMON_H
#define RSCOMMON_H
#include <QDir>
#include <QString>
#include <QDateTime>
#include <QDebug>
#ifdef WIN32
#define __FUNCTION_NAME__ __FUNCSIG__
#else
#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#endif //WIN32
QString TimeToString();

static const QString s_RSDMLogPath = QString(QDir::homePath() + "/.rslogger/");
static const QString scLibraryName = QString("RapidshareDownloadManager");

static const QString scRsdm_StateMachineStateStopped = QString ( " Stalled " ) ;
static const QString scRsdm_StateMachineStateGet_First = QString ( " Shaking with rapidshare.com " ) ;
static const QString scRsdm_StateMachineStateGet_Second = QString ( " Requesting second GET " ) ;
static const QString scRsdm_StateMachineStateGet_Third = QString ( " Requesting third GET " ) ;
static const QString scRsdm_StateMachineStatePost_First = QString ( " Requesting first POST " ) ;
static const QString scRsdm_StateMachineStateDone = QString ( " Downloading finished " ) ;
static const QString scRsdm_StateMachineStateFailed = QString ( " Downloading failed " ) ;


#ifndef LOG_FUNC_TO_OUT
#define LOG_FUNC_TO_OUT			qDebug() << TimeToString() << __FUNCTION_NAME__ 
#endif
#endif //RSCOMMON_H
