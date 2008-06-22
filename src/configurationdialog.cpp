//
// C++ Implementation: configurationdialog
//
// Description: 
//
//
// Author: Bartek Taczała <b@kontrasty.szczecin.pl>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent )
 : QDialog(parent)
{
	contentsWidget = new QListWidget;
	contentsWidget->setViewMode(QListView::IconMode);
	contentsWidget->setIconSize(QSize(96, 84));
	contentsWidget->setMovement(QListView::Static);
	contentsWidget->setMaximumWidth(128);
	contentsWidget->setSpacing(12);

	pagesWidget = new QStackedWidget;
	pagesWidget->addWidget(new ConfigurationPage);
	pagesWidget->addWidget(new UpdatePage);
	pagesWidget->addWidget(new QueryPage);

	QPushButton *closeButton = new QPushButton(tr("Close"));

	createIcons();
	contentsWidget->setCurrentRow(0);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(contentsWidget);
	horizontalLayout->addWidget(pagesWidget, 1);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(closeButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(12);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Config Dialog"));
}

void ConfigurationDialog::createIcons()
{
	QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
	configButton->setIcon(QIcon(":/configure.png"));
	configButton->setText(tr("Configuration"));
	configButton->setTextAlignment(Qt::AlignHCenter);
	configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
	updateButton->setIcon(QIcon(":/configure.png"));
	updateButton->setText(tr("Update"));
	updateButton->setTextAlignment(Qt::AlignHCenter);
	updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
	queryButton->setIcon(QIcon(":/configure.png"));
	queryButton->setText(tr("Query"));
	queryButton->setTextAlignment(Qt::AlignHCenter);
	queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	connect(contentsWidget,SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigurationDialog::changePage(QListWidgetItem * current, QListWidgetItem * previous)
{
	if(current)
		current = previous;
	pagesWidget->setCurrentIndex(contentsWidget->row(current));
}



