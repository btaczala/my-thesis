/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza³a 								   *
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
#ifndef QAPPSTYLE_H
#define QAPPSTYLE_H
#include <QCommonStyle>
class QAppStyle : public QCommonStyle 
{
	Q_OBJECT
public:
	QAppStyle(){};
	~QAppStyle() throw() {};
	virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w  = 0 ) const ;

};

class QMenuBarStyle : public QCommonStyle
{
	Q_OBJECT
public:
	QMenuBarStyle() : QCommonStyle() {};
	~QMenuBarStyle() throw() {} ;
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *opt, QPainter *painter, const QWidget *widget = 0	) const ;
	virtual void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w  = 0 ) const ; 
};

#endif // QAPPSTYLE_H