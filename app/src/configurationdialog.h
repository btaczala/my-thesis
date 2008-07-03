//
// C++ Interface: configurationdialog
//
// Description: 
//
//
// Author: Bartek Taczała <b@kontrasty.szczecin.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QtGui>
#include <QDialog>
#include "pages.h"

/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/
class ConfigurationDialog : public QDialog
{
	Q_OBJECT
public:
	ConfigurationDialog(QWidget *parent);
public slots:
	void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
	
	void 				createIcons();
	QListWidget 		*contentsWidget;
	QStackedWidget		*pagesWidget;
};
#endif

