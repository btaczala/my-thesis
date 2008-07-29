//
// C++ Interface: common
//
// Description: 
//
//
// Author: Bartek Taczała <b@kontrasty.szczecin.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMON_H
#define COMMON_H
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

#define APPLICATION_NAME		"qrapidshare"

#define	UI_WINDOW_SIZE			"mainwindow/size"
#define	UI_WINDOW_POS			"mainwindow/pos"

static const QString scSettingsPath_UserName = QString("user/username");
static const QString scSettingsPath_UserPass = QString("user/userpass");
static const QString scSettingsPath_MaxDownloads = QString("manager/max_downloads");
static const QString scSettingsPath_DefaultPath = QString("manager/default_path");
static const QString scSettingsPath_DownloadlistSize = QString("manager/download_list_elements");
static const QString scSettingsPath_DownloadlistRoot = QString("manager/download_list/") ;
//static const QString scSettingsPath_DownloadlistElements = QString("manager/download_list/%1/") ;
static const QString scSettingsPath_DownloadUrlFilePath = QString("manager/download_list/%1/UrlPath") ;
static const QString scSettingsPath_DownloadFileDestination = QString("manager/download_list/%1/FileDest") ;
static const QString scSettingsPath_DownloadFileState = QString("manager/download_list/%1/State") ;
static const QString scSettingsPath_DownloadFilePercentage = QString("manager/download_list/%1/Progress") ;
#ifndef LOG_FUNC_TO_OUT
#define LOG_FUNC_TO_OUT			qDebug() << TimeToString() << __FUNCTION_NAME__ 
#endif
#endif
