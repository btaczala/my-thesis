/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza?a 								   *
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
#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <vector> 
#include <boost/shared_ptr.hpp>
#include <QObject>

class IDownload ; 
class DownloadEngine ;
class DownloadManager : public QObject
{
    Q_OBJECT
	public:
		DownloadManager() ; 
		~DownloadManager() ; 
        void                addDownload( const std::string & urlAddress, const std::string & destination ) ;
        void                startDownload( const std::string & urlAddress ) ;
        void                removeDownload( const std::string & urlAddress ) ;
	private : 
        std::auto_ptr<std::vector<boost::shared_ptr<IDownload> > 	> 		m_apDownloadList ; 
        unsigned int        m_iMaxDownloadFiles ; 
        unsigned int        m_iCurrentDownloadingFiles ; 
        std::vector< boost::shared_ptr<DownloadEngine> >                    m_apDownloadEngines ; 
        
        IDownload *         find(const std::string & pattern ) ; 
};
#endif // DOWNLOADMANAGER_H
