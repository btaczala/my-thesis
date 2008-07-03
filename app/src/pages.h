//
// C++ Interface: pages
//
// Description: 
//
//
// Author: Bartek Taczała <b@kontrasty.szczecin.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAGES_H
#define PAGES_H
#include <QWidget>

class ConfigurationPage : public QWidget
{
	public:
		ConfigurationPage(QWidget *parent = 0);
};

class QueryPage : public QWidget
{
	public:
		QueryPage(QWidget *parent = 0);
};

class UpdatePage : public QWidget
{
	public:
		UpdatePage(QWidget *parent = 0);
};
class UserPage : public QWidget
{
	public:
		UserPage(QWidget *parent = 0);
};
#endif
