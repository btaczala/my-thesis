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
#ifndef IDOWNLOADMANAGER_H
#define IDOWNLOADMANAGER_H
#include <memory>
#include <string>
/*! \struct DownloadState
 * \brief Holds information about bytes read. 
 */
struct DownloadState
{
public:
	typedef unsigned int UInt ; 
	typedef unsigned long ULong ; 
	UInt 					m_DownloadFileSize ; 
	UInt					m_BytesDownloaded ; 
	int						bytesReadCurrent;
	int						bytesReadPreviously;
	unsigned long			m_bytesToRead ;
	unsigned long			m_Percentage ; 
    enum States
    {
        STOPPED=0,
        DOWNLOADING,
        PAUSED,
        FINISHED
    } m_State ; 
};
class IDownload 
{
	public :
		IDownload () ;
		~IDownload() ;
        DownloadState * 	downloadInfo () const ; 
		virtual void 		start() = 0 ; 
		virtual void 		pause() = 0 ; 
		virtual void 		stop() = 0 ; // abort () 
		virtual void 		restart() = 0 ; 
		
		void                setUrlAddress ( const std::string & urlAddrr ) ; 
		const std::string & urlAddress() const ; 
		const std::string & destinationAddress() const ; 
		void                setDestinationAddress ( const std::string & localAddress  ) ; 
	private :
        std::auto_ptr< DownloadState >		m_pDownloadInfo ; 
		std::string						    m_UrlAddress ; 
		std::string						    m_FileDestination ; 		
};
#endif //  IDOWNLOADMANAGER_H