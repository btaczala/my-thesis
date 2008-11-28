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
        /**
         * @brief Will add download into queue
         * @param urlAddress url address of file to download
         * @param destination where to put the file ( directory only ) 
         */
        void                    addDownload( const std::string & urlAddress, const std::string & destination ) ;
        /**
         * @brief Will force on manager to start downloading this file
         * @param urlAddress url address of file to download
         */
        void                    startDownload( const std::string & urlAddress ) ;
        /**
         * @brief will remove download 
         * @param urlAddress 
         */
        void                    removeDownload( const std::string & urlAddress ) ;
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
    private : 
        DownloadListType        m_DownloadList ; 
        unsigned int            m_iMaxDownloadFiles ; 
        unsigned int            m_iCurrentDownloadingFiles ; 
        struct 
        {
            unsigned int m_CurrentDownloadingFiles ; 
            unsigned int m_MaxDownloadingFiles ; 
            unsigned int m_LastStartedFilePosition; 
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
    private slots:
        void                    slot_listChanged() ; 
        void                    init();
        void                    statusChanged(DownloadState::States what);
        void                    bytesRead(int read,int total);
        void                    downloadDone();
        void                    downloadRate(const QString & dwnlRate);
        /**
         * @brief  Updates list of download. 
         * This signal can be invoked by others. 
         */
        void                    update() ; 
    signals:
        /**
         * Will emit global progress. 
         * @param value 
         */
        void                    globalProgress(int value);
        /**
         * Will emit that download at 'at' changed its state. 
         * @param at - download position
         * @param state - changed state
         */
        void                    statusChanged ( int at, DownloadState::States state  ) ; 
        
        /**
         * Will emit that download at 'at' postion finished downloading. 
         * @param at 
         */
        void                    downloadDoneAt ( int at ) ;
         
        
        /**
         * Will emit if download at 'at' for some reasons will stop ( not user action )
         * Probably network error, or drive full
         * @param at 
         */
        void                    downloadOnHold( int at ) ;
        
        /**
         * Will emit bytes downloaded 
         * @param at - position
         * @param read - how many have we read
         * @param total - how big is file. 
         */
        void                    bytesReadAt ( int at, int read, int total ) ; 
};
#endif // DOWNLOADMANAGER_H
