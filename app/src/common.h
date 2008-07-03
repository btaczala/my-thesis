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

#define	UI_WINDOW_SIZE			"mainwindow/size"
#define	UI_WINDOW_POS			"mainwindow/pos"

#define LOG_FUNC_TO_OUT			qDebug() << TimeToString() << __PRETTY_FUNCTION__ 

#endif
