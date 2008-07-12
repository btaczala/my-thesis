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

#ifndef LOG_FUNC_TO_OUT
#define LOG_FUNC_TO_OUT			qDebug() << TimeToString() << __PRETTY_FUNCTION__ 
#endif
#endif
