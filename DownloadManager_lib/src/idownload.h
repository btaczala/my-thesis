/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza?a                                  *
 *   b@kontrasty.szczecin.pl                                               *
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

#ifndef IDOWNLOAD_H
#define IDOWNLOAD_H
#include <memory>
#include <string>
#include <QObject>


/*! \struct DownloadState
 * \brief Holds information about bytes read. 
 */
struct DownloadState
{
public:
    DownloadState() : m_DownloadFileSize(0),m_BytesDownloaded(0),bytesReadCurrent(0),bytesReadPreviously(0),m_bytesToRead(0),m_Percentage(0), m_State(STOPPED){};
    typedef unsigned int UInt ; 
    typedef unsigned long ULong ; 
    UInt                    m_DownloadFileSize ; 
    UInt                    m_BytesDownloaded ; 
    int                     bytesReadCurrent;
    int                     bytesReadPreviously;
    unsigned long           m_bytesToRead ;
    unsigned long           m_Percentage ;  
    enum States
    {
        // ?: is it a proper state ? 
        // with this commented out, qrapidshare engine doesn't work. ( response html is written as result file ). 
        INIT = 0,
        STOPPED ,
        DOWNLOADING,
        PAUSED,
        DONE,
        FAILED
    } m_State ; 
};

QString DownloadStateToString( DownloadState::States state ) ;
class OptionsContainer;

class IDownload : public QObject
{
    Q_OBJECT
    public :
        IDownload (OptionsContainer* options = NULL );
        virtual ~IDownload() ;
        DownloadState*                      downloadInfo () const ; 
        virtual void                        start() = 0 ; 
        virtual void                        stop() = 0 ; // abort () // it's pause
        virtual void                        restart() = 0 ;
        DownloadState::States               state() const {return m_pDownloadInfo->m_State; };
        //void                                SetState(const DownloadState::States& _state ) { m_pDownloadInfo->m_State = _state; };
        void                                setBytesDownloaded(unsigned int _bytes ){ m_pDownloadInfo->m_BytesDownloaded = _bytes; };
        unsigned int                        getBytesDownloaded() const {    return m_pDownloadInfo->m_BytesDownloaded ;  };
        unsigned int                        fileSize() const { return m_pDownloadInfo->m_DownloadFileSize; };
        void                                setFileSize( const unsigned int & fileSize ) {  m_pDownloadInfo->m_DownloadFileSize = fileSize ; };
        unsigned int                        GetProgress() const;
        
        void                                setUrlAddress ( const std::string & urlAddrr ) ; 
        const std::string&                  urlAddress() const ; 
        const std::string&                  destinationAddress() const ; 
        void                                setDestinationAddress ( const std::string & localAddress  ) ;
        const std::string                   error() ; 
        void                                setState(const DownloadState::States& _state, bool triggerEmit = false);
    
    protected:
        void                                setFileName();
        void                                calculateProgress( qint64 done, qint64 total );
        
    protected:
        
        std::string                         m_UrlAddress ; 
        std::string                         m_FileDestination ;
        std::string                         m_FileName;
        std::string                         m_Error ; 
        mutable unsigned int                m_Progress;
        OptionsContainer*                   m_Options;
        unsigned int                        m_SecondsDownloading ;
    private:
        std::auto_ptr< DownloadState >      m_pDownloadInfo ;
        
    signals :
        //virtual void                        downloadStatus(const int & istate ) = 0;
        virtual void                        bytesRead( int read, int howMany ) = 0 ; 
        virtual void                        statusChanged( DownloadState::States status ) = 0 ;
        virtual void                        downloadRate( const QString & dwnlRate) = 0 ; 
        virtual void                        elapsedTime( unsigned int elapsedTime ) = 0 ; 
        
};
#endif //  IDOWNLOAD_H
