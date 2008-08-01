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
#include "qappstyle.h"
#include <QStyleOption>
#include <QPainter>

void QAppStyle::drawPrimitive( PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w ) const 
{
	if ( pe == QStyle::PE_PanelMenuBar || pe == QStyle::PE_FrameMenu || pe == QStyle::PE_Frame)
	{
		QPolygon points(3);
		int x = opt->rect.x();
		int y = opt->rect.y();
		int w = opt->rect.width() / 2;
		int h = opt->rect.height() / 2;
		x += (opt->rect.width() - w) / 2;
		y += (opt->rect.height() - h) / 2;

		if (pe == PE_IndicatorSpinUp) {
			points[0] = QPoint(x, y + h);
			points[1] = QPoint(x + w, y + h);
			points[2] = QPoint(x + w / 2, y);
		} else { // PE_SpinBoxDown
			points[0] = QPoint(x, y);
			points[1] = QPoint(x + w, y);
			points[2] = QPoint(x + w / 2, y + h);
		}

		if (opt->state & State_Enabled) {
			p->setPen(opt->palette.mid().color());
			p->setBrush(opt->palette.buttonText());
		} else {
			p->setPen(opt->palette.buttonText().color());
			p->setBrush(opt->palette.mid());
		}
		p->drawPolygon(points);
	}
	else
//		QCommonStyle::drawPrimitive(pe,opt,p,w);
;
}

void QMenuBarStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *opt, QPainter *painter, const QWidget *widget  ) const 
{
	// here we draw MenuBar items ( likes QAction ) 
	QCommonStyle::drawPrimitive(element,opt,painter,widget);
}

void QMenuBarStyle::drawControl( ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w  ) const
{
	//here we draw MenuBar ( likes QMenu )
	//QCommonStyle::drawControl(element,opt,p,w);
}