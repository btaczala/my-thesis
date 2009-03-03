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
#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <vector> 
#include <map>
#include <boost/shared_ptr.hpp>

#include <QObject>
#include <QMutex>

#include "idownload.h"
#include "progressinfo.h"


class IDownload ; 
class DownloadEngine ;
class EngineManager;

class DownloadManager : public QObject
{
    Q_OBJECT
    public:
        /**
         * @var IDownloadSmartPtrs - smart pointer of IDownload
         */
        typedef boost::shared_ptr<IDownload> IDownloadSmartPtr ; 
        /**
         * @var DownloadListType - type of container 
         */
        typedef std::vector<IDownloadSmartPtr> DownloadListType ; 
        DownloadManager() ; 
        ~DownloadManager() ; 
        void                    init();
        /**
         * @brief Will add download into queue
         * @param urlAddress url address of file to download
         * @param destination where to put the file ( directory only ) 
         * @return download currently added. If NULL - download was not added to list 
         */
        const IDownload * addDownload( const std::string & urlAddress, const std::string & destination ) ;
        /**
         * @brief Will force on manager to start downloading this file
         * @param urlAddress url address of file to download
         */
        void                    startDownload( const std::string & urlAddress ) ;
        /**
         * @brief Will force on manager to start downloading this file
         * @param position position within list  
         */
        void                    startDownload( int position ) ;
        /**
         * @brief Will force on manager to stop downloading this file
         * @param urlAddress 
         */
        void                    stopDownload ( const std::string & urlAddress ) ; 
        /**
         * @brief Will force on manager to stop downloading this file
         * @param position position within list  
         */
        void                    stopDownload ( int position ) ;
        /**
         * @brief Start is download is paused, pause if it's started. :) 
         * This is the only method which can invoke IDownload::resume() method. 
         * @param position position within list  
         */
        void                    startPause ( int position ) ; 
        void                    startPause ( const std::string & urlAddress  ) ; 
        
        /**
         * @brief will remove download 
         * @param urlAddress 
         */
        void                    removeDownload( const std::string & urlAddress ) ;
        /**
         * @brief will remove download 
         * @param position position within list  
         */
        void                    removeDownload( int position ) ;

        /**
         * @brief Will return engine manager 
         * @return 
         */
        const EngineManager *   engineManager() const ; 
        /**
         * Will return percentage of all download 
         * @return 
         */
        int                     percentage() ; 
        enum DownloadManagerState
        {
            DOWNLOADING = 0, 
            STOPPED,
        };
        /**
         * @brief Changes state of Download manager 
         * Will change current state of download manager. If state is changed, update() will be invoked. 
         * @param state 
         */
        void setState ( DownloadManagerState state ) ; 
        DownloadManagerState state () const ; 

        const IDownload *downloadAt ( unsigned int position ) ; 
        
        
    private : 
        DownloadListType        m_DownloadList ; 
        unsigned int            m_iMaxDownloadFiles ; 
        unsigned int            m_iCurrentDownloadingFiles ; 
        struct 
        {
            unsigned int m_CurrentDownloadingFiles ; 
            unsigned int m_MaxDownloadingFiles ; 
        } m_DownloadManagerSettings ; 
        DownloadManagerState m_State ; 
        std::auto_ptr<EngineManager>   m_pEngineManager;
        QMutex                         m_Mutex ; 
        /*
            FUNCTIONS
        */
        /**
         * find download by url 
         * @param pattern 
         * @return 
         */
        IDownload *             find(const std::string & pattern ) ;
        /**
         * Find position of download 
         * @param url 
         * @return 
         */
        int                     findPosition( const std::string & url ) ; 
        /**
         * Connect QObject signal/slot mechanism with signals from pDownload
         * @param pDownload 
         */
        void                    connectWith( IDownload *pDownload ) ;
        void                    disconnectWith( IDownload *pDownload ) ; 
        /**
         * Return position of download in container which send a signal
         * @param sender - Download that sends the signal. Will be casted to IDownload*
         * @return position in container. If not found -1. 
         */
        int                     getPositionWithinSlot( QObject * sender );
        /**
         * @brief Decides if download is possible
         * Check download manager state, and decides if we can start next download. 
         * This has to be invoked this lock
         * @return true if we can start next download
         */
        bool                    canIDownload() const ;
        
        void                    decreaseNumberOfCurrentDownloads();
        void                    increaseNumberOfCurrentDownloads();
        
        void                    startDownloadAndConnect ( IDownload *pDownload ) ;
        void                    stopDownloadAndDisconnect(IDownload *pDownload ) ;
    private slots:
        void                    slot_listChanged() ; 

        void                    statusChanged(DownloadState::States what);
        //void                    bytesRead(int read,int total);
        //void                    downloadDone();
        //void                    downloadRate(const QString & dwnlRate);
        //void                    elapsedTime( unsigned int elapsedTime );
        void                    progressInfo( const ProgressInfo& _info);
    public slots:
        /**
         * @brief  Updates list of download. 
         * This signal can be invoked by others. 
         */
        void                    update() ; 
    signals:
        /**
         * Will emit that download at 'at' changed its state. 
         * @param at - download position
         * @param state - changed state
         */
        void                    statusChanged ( int at, DownloadState::States state  ) ; 
        
        /**
         * Will emit bytes downloaded 
         * @param at - position
         * @param read - how many have we read
         * @param total - how big is file. 
         */
        void                    progressInfoAt( int at, const ProgressInfo& _info );
		/**
		 * signal downloadAdded() 
		 * Will emit when some frontend will add some download to queue. 
		 */
		void					downloadAdded( int newPosition ) ; 
		/**
		 * signal downloadRemoved() 
		 * Will emit when some frontend will remove download from queue. 
		 */
		void					downloadRemoved( int position ) ; 
};
#endif // DOWNLOADMANAGER_H
