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
QString TimeToString();

#define APPLICATION_NAME		"qrapidshare"
#define SET_USERNAME			"user/username"
#define SET_USERPASSWORD		"user/userpass"
#define	UI_WINDOW_SIZE			"mainwindow/size"
#define	UI_WINDOW_POS			"mainwindow/pos"
#define RSM_MAX_DOWNLOAD		"manager/max_downloads"
#define RSM_DEF_DOWNLOADS_PATH	"manager/default_path"


#define RS_LOG_PATH				QDir::homePath() + "/.rslogger/"
#define LOG_FUNC_TO_OUT			qDebug() << TimeToString() << __PRETTY_FUNCTION__ 

#endif
