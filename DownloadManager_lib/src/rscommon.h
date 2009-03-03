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
/*! \enum RapidShareStateMachine
* \brief hold state machine state. 
* 
*/
//enum rapidsharestatemachine
//{
//	stopped = 0,
//	get_first,
//	get_second,
//	post_first,
//	get_third, // aka downloading 
//	downloading,
//	done,
//	failed,
//	paused
//};

//! \fn StateToString - translating RapidShareStateMachine into QString 
 
//
//QString StateToString(const RapidShareStateMachine & rsMachineState);
//QString StateToString2(const RapidShareStateMachine & rsMachineState);
//RapidShareStateMachine StringToState(const QString & string );

static const QString s_RSDMLogPath = QString(QDir::homePath() + "/.rslogger/");
static const QString scLibraryName = QString("RapidshareDownloadManager");

static const QString scRsdm_StateMachineStateStopped = QString ( " Stalled " ) ;
static const QString scRsdm_StateMachineStateGet_First = QString ( " Shaking with rapidshare.com " ) ;
static const QString scRsdm_StateMachineStateGet_Second = QString ( " Requesting second GET " ) ;
static const QString scRsdm_StateMachineStateGet_Third = QString ( " Requesting third GET " ) ;
static const QString scRsdm_StateMachineStatePost_First = QString ( " Requesting first POST " ) ;
static const QString scRsdm_StateMachineStateDone = QString ( " Downloading finished " ) ;
static const QString scRsdm_StateMachineStateFailed = QString ( " Downloading failed " ) ;

static const QString scRsdm_SettingsOrganization = QString( "QRsdm" ) ;
static const QString scRsdm_SettingsAppName = QString( "QtRapidshareDownloadManager_corelib" ) ;
static const QString scRsdm_SettingsDownloadList = QString( "Download_list" ) ;
static const QString scRsdm_SettingsDownloadListSize = QString( "Download_list/size" ) ;
static const QString scRsdm_SettingsDownloadListUrlPath = QString( "Download_list/%1/UrlPath" ) ;
static const QString scRsdm_SettingsDownloadListFileDest = QString( "Download_list/%1/FileDest" ) ;
static const QString scRsdm_SettingsDownloadListState = QString( "Download_list/%1/State" ) ;
static const QString scRsdm_SettingsDownloadListPercentage = QString( "Download_list/%1/bytesDownloaded" ) ;
static const QString scRsdm_SettingsDownloadListFileSize = QString( "Download_list/%1/FileSize" ) ;
static const QString scRsdm_SettingsDownloadListHost = QString( "Download_list/%1/Host" ) ;

static const int scDefaultContentValue = 524288 ; 

#endif //RSCOMMON_H
