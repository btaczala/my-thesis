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
#include <QFile>

#include "progressinfo.h"


/*! \struct DownloadState
 * \brief Holds information about bytes read. 
 */
struct DownloadState
{
public:
    DownloadState() /*m_DownloadFileSize(0),m_BytesDownloaded(0),bytesReadCurrent(0),bytesReadPreviously(0),m_bytesToRead(0),m_Percentage(0), */
    : m_State(STOPPED)
    , m_DownloadedBytes(0)
    , m_TotalBytes(0)
    , m_PrevDownloadedBytes(0){};
    
    qint64                  m_DownloadedBytes;
    qint64                  m_TotalBytes;
    qint64                  m_PrevDownloadedBytes;

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

namespace Download
{
    static int TIMERINT = 1000; //1sek
    static const QString TMPSTRING = ".part";
}

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
        qint64                              totalBytes() const { return m_pDownloadInfo->m_TotalBytes; };
        qint64                              downloadedBytes() const { return m_pDownloadInfo->m_DownloadedBytes; };
        
        void                                setUrlAddress ( const std::string & urlAddrr ) ; 
        const std::string&                  urlAddress() const ; 
        const std::string&                  destinationAddress() const ; 
        void                                setDestinationAddress ( const std::string & localAddress  ) ;
        const std::string                   error() const { return m_Error;}; 
        void                                setState(const DownloadState::States& _state, bool triggerEmit = false);
        void                                emitStatusChanged(){ emit( statusChanged(m_pDownloadInfo->m_State));};
        unsigned int                        progress() const;
    
    protected:
        void                                setFileName();
        void                                initFile();
        void                                removeFromFile( const QString& _post );
        void                                renameFile();
        void                                closeFile();
        bool                                openFile();
        QIODevice*                          file(){ return m_apFile.get(); };
        const std::string&                  fileName(){ return m_FileName; };
        bool                                isFileOpen();
        qint64                              writeToFile( const char* _data, qint64 _amount );
        void                                calculateProgress( qint64 _done, qint64 _total ){ 
                                                                m_pDownloadInfo->m_TotalBytes = _total;
                                                                m_pDownloadInfo->m_DownloadedBytes = _done; };
        void                                setTotalBytes( qint64 _total ){ m_pDownloadInfo->m_TotalBytes = _total; };
        void                                setDownloadedBytes( qint64 _done ){ m_pDownloadInfo->m_DownloadedBytes = _done; };
        void                                setError( const std::string& _err );  
        void                                timerEvent(QTimerEvent* event);
    protected:
        
        std::string                         m_UrlAddress ; 
        std::string                         m_FileDestination ;
        std::string                         m_Error ; 
        OptionsContainer*                   m_Options;
        std::string                         m_FileName;
        
    private:
        std::auto_ptr< DownloadState >      m_pDownloadInfo ;
        int                                 m_TimerId;
        unsigned int                        m_SecondsDownloading ;
    protected:
        std::auto_ptr<QFile>                m_apFile;
        
    signals :
        void                        downloadStatus(const int & istate );
        void                        statusChanged( DownloadState::States status );
        void                        progressInfo( const ProgressInfo& _info );
};
#endif //  IDOWNLOAD_H
