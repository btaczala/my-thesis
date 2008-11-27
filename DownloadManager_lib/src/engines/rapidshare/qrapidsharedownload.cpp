#include <QDebug>
#include <boost/any.hpp>

#include "qrapidsharedownload.h"
#include "optionscontainer.h"
#include "rscommon.h"

 
 
QRapidshareDownload::QRapidshareDownload(OptionsContainer* options): IDownload(options)
 , m_apHttpObj( new QHttp() )
 , m_apHttpRequestHeader(new QHttpRequestHeader() )
 , m_apRSUser(NULL)
 , m_apFileUrl( new QUrl() )
 , m_apFile(new QFile() )
 , m_timerId(0)
 , m_readedBytes(0)
 , m_Logger(QString("qrapidsharedownload") + QString::number(qrand() ) )
 
 {
    RSDM_LOG_FUNC ;
    QObject::connect( m_apHttpObj.get(), SIGNAL( requestStarted( int ) ), this, SLOT( requestStarted( int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( stateChanged( int ) ), this, SLOT( stateChanged( int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( dataSendProgress( int,int ) ), this, SLOT(  dataSendProgress( int,int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( authenticationRequired(  const QString , quint16 , QAuthenticator *) ), this, SLOT(  authenticationRequired(  const QString , quint16 , QAuthenticator *)  ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ), this, SLOT(  proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( readyRead ( QHttpResponseHeader ) ), this, SLOT(  readyRead ( QHttpResponseHeader ) ) );   

    m_errorsList.append(RsErrors::err2);
    m_errorsList.append(RsErrors::err5);

    if( m_Options )
    {
        setUser(  boost::any_cast<std::string>( m_Options->getOption( scRS_USER_NAME )),
                  boost::any_cast<std::string>( m_Options->getOption( scRS_USER_PASS )));
    }
    else
        Q_ASSERT(false);
 }

 QRapidshareDownload::~QRapidshareDownload()
 {
    RSDM_LOG_FUNC ;
    m_apHttpObj.get()->disconnect();
 }

 void QRapidshareDownload::start()
 {
    RSDM_LOG_FUNC ;
    
    setUrlFileAddress( m_UrlAddress.c_str() );
    //invalid url set
    if( m_apFileUrl->isEmpty())
        return;

    m_ReferrerFileAddress = m_UrlAddress.c_str();

    QString fileName(m_FileDestination.c_str());
    fileName += "/";
    fileName += transformUrlPathToLocalPath(m_ReferrerFileAddress);
    if( fileName.isEmpty())
       return;
    
    m_qFileDestination = fileName;

    m_apFile->setFileName( fileName + ".part");

    if ( QFile::exists(m_apFile->fileName()) )
    {
         // resume downloading 
    }
    m_rssmState = GET_FIRST;
    m_pDownloadInfo->m_State = DownloadState::INIT;
    emit statusChanged( m_pDownloadInfo->m_State );

    m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
    m_apHttpRequestHeader->setValue("Host",  m_apFileUrl->host() );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
    m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50");
    m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress );
    qDebug() << QString("First GET");
    qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
    m_apHttpObj->setHost(  m_apFileUrl->host() );
    m_apHttpObj->request( *( m_apHttpRequestHeader ) );
 }

 void QRapidshareDownload::stop()
 {
    RSDM_LOG_FUNC ;
    if( m_pDownloadInfo->m_State  == DownloadState::STOPPED
        || m_pDownloadInfo->m_State  == DownloadState::DONE
        || m_pDownloadInfo->m_State  == DownloadState::FAILED)
        return ; 
    m_pDownloadInfo->m_State  = DownloadState::PAUSED ;
    m_apHttpObj->abort();
    emit statusChanged( m_pDownloadInfo->m_State );
 }

 void QRapidshareDownload::restart()
 {
    RSDM_LOG_FUNC ;
    //m_apHttpRequestHeader->removeValue();
    m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
    m_apHttpRequestHeader->setValue("Host", m_DownloadServerHost );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
    m_apHttpRequestHeader->setValue("Range", "bytes=" + QString::number(m_pDownloadInfo->m_BytesDownloaded)+ "-" );
    m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; MSIE 5.5; Windows 98)");
    m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress);
    qDebug() << QString("Resumed !!!!!!");
    QString httpHeader = DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
    qDebug() << httpHeader;
    QString host = m_apFileUrl->host();
    m_apHttpObj->setHost( m_DownloadServerHost );
    m_apHttpObj->request( *( m_apHttpRequestHeader ) );
 }

 void QRapidshareDownload::setUrlFileAddress(const QString & _addr )
 {
    RSDM_LOG_FUNC ;
    DebugUtils::q_Log( QString(" _addr = ") + _addr);
    if( ! _addr.isEmpty() )
    {
        m_apFileUrl.reset( new QUrl( _addr ) );
    }
 }

 /********** SLOTS **************/
 void QRapidshareDownload::requestStarted(const int & idReq)
 {
    qDebug() << __FUNCTION_NAME__<< "idReq =  " << idReq ;
 }
 void QRapidshareDownload::requestFinished(const int & idReq, const bool & isFalse)
 {  
    qDebug() << __FUNCTION_NAME__<< "idReq =  " << idReq << "isFalse=" << isFalse ;
    m_apFile->close();
    if( isFalse )
    {
        qDebug() << m_apHttpObj->errorString() ;
        return ; 
    }
 }
 void QRapidshareDownload::stateChanged(const int & iState)
 {  
    qDebug() << __FUNCTION_NAME__<< "iState =  " << iState ;
 }
 void QRapidshareDownload::dataSendProgress(const int & done, const int & total)
 {  
    qDebug() << __FUNCTION_NAME__<< "done =  " << done << "total=" << total ;
 }
 void QRapidshareDownload::dataReadProgress(const int & done, const int & total)
 {
    qDebug() << __FUNCTION_NAME__<< "done =  " << done << "total=" << total ;
    // support for Direct downloads
    char *buff = NULL ; 
    qint64 iBytes2 = 0;
    qint64 bytes = m_apHttpObj->bytesAvailable();
    m_readedBytes+=bytes;
    if( m_rssmState == GET_THIRD || m_rssmState == POST_FIRST )
    {
        buff = new char[bytes];
        iBytes2 = m_apHttpObj->read(buff, bytes);
        if( -1 == iBytes2 )
            qDebug()<<("ERROR while reading from Http stream ");
        QString decive ( buff ) ;
        if( decive.contains("<!DOCTYPE"))
        {
            // this is still html. just do nothing 
            return ; 
        }
        m_rssmState = DOWNLOADING;
        m_pDownloadInfo->m_State = DownloadState::DOWNLOADING;
        emit statusChanged( m_pDownloadInfo->m_State ) ; 
        m_timerId = startTimer(1000);
        m_readedBytes = 0;
    }
    if( m_pDownloadInfo->m_State == DownloadState::PAUSED )
    {
        if(m_apFile.get() !=NULL)
        {
            if(!m_apFile->isOpen())
            {
                qDebug() << "Openning file to resume " ; 
                if(!m_apFile->open(QIODevice::WriteOnly | QIODevice::Append ) )
                {
                    qDebug() << "UnSuccessfull opening file " ; 
                    return ; 
                }
                m_pDownloadInfo->m_State = DownloadState::DOWNLOADING;
                emit statusChanged( m_pDownloadInfo->m_State );
                m_timerId = startTimer(1000);
                m_readedBytes = 0; 
            }
        }
    }
    if ( m_pDownloadInfo->m_State  == DownloadState::DOWNLOADING ) 
    {
        //m_pDownloadInfo->m_FileInfo.m_FileSize = total ; 
        // just past it to the file ! 
        int bytesDownloadedOverall = (m_pDownloadInfo->m_DownloadFileSize - total) > 0 ?  m_pDownloadInfo->m_DownloadFileSize - total : 0 ; 
        m_pDownloadInfo->m_BytesDownloaded = done + bytesDownloadedOverall; 
        double dDone = m_pDownloadInfo->m_BytesDownloaded;
        double dTotal = m_pDownloadInfo->m_DownloadFileSize;
        double dResTotal = dDone / dTotal;
        dResTotal *= 100;
        m_Progress = (int)dResTotal;
        qDebug() << "emit :DownloadStatus("<< m_Progress <<")";  
        emit bytesRead(dDone,dTotal) ;
        m_pDownloadInfo->bytesReadPreviously =m_pDownloadInfo->bytesReadCurrent;
        m_pDownloadInfo->bytesReadCurrent = done ; 
        if ( buff == NULL ) 
        {
            buff = new char[bytes];
            iBytes2 = m_apHttpObj->read(buff, bytes);
        }
        if ( -1 == iBytes2)
            qDebug()<<("ERROR while reading from Http stream ");
 
        else
        {
            qDebug() << "readed bytes " << iBytes2;
            if( !m_apFile->isOpen())
            {
                qDebug() << ("isClosed, open it");
                if( ! m_apFile->open(QIODevice::WriteOnly | QIODevice::Append) )
                {
                    qDebug()<<("ERROR '-_- "); //. 
                    // message = bad
                    // 
                    return ;
                }
                else
                    qDebug() << "opened";
            }
 
            qint64 btmp = m_apFile->write(buff,iBytes2); 
            if( -1 == btmp )
            {
                qDebug()<<("write failed");
                return;
            }
            else
                qDebug() << ("written successful") << btmp;
        }
        delete[] buff;
    }
 
    if( done == total &&  m_rssmState ==  DOWNLOADING ) 
    {
        m_pDownloadInfo->m_State = DownloadState::DONE;
        m_rssmState = FINISHED;
    }
 }
 void QRapidshareDownload::authenticationRequired(const QString & hostname, quint16 port, QAuthenticator * authenticator)
 {
    qDebug() << __FUNCTION_NAME__<< "hostname =  " << hostname << "port=" << port ;
 }
 void QRapidshareDownload::proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator)
 {
    qDebug() << __FUNCTION_NAME__;
 }
 void QRapidshareDownload::readyRead ( const QHttpResponseHeader & resp )
 {
    qDebug() << __FUNCTION_NAME__;
 }
 
 void QRapidshareDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
 {
    RSDM_LOG_FUNC ;
    qDebug() << resp.reasonPhrase() ;
    int iStatusCode = resp.statusCode(); 
    if( iStatusCode == 200 || iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 303 || iStatusCode == 307 )
    {
        ;
    }
    else
        qDebug() << "Error response:"<< iStatusCode;
 }
 void QRapidshareDownload::done(const bool & error)
 {
    RSDM_LOG_FUNC ;
    if( error )
    {
        qDebug() << m_apHttpObj->errorString();
        return ;
    }

    if( m_rssmState == GET_FIRST )
    {
        QByteArray aa = m_apHttpObj->readAll();
        
        if( checkForErrors( aa ))
        {
            m_pDownloadInfo->m_State = DownloadState::FAILED;
            emit statusChanged( m_pDownloadInfo->m_State );
            return;
        }

        DebugUtils::DumpReponseToFile(aa,"get_first");
        m_apHttpRequestHeader->removeValue("Cookie");
        m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Accept", "image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, ap//plication/vnd.ms-powerpoint, application/msword, application/x-shockwave-flash, */*" );
        m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress );
        qDebug() << "Second GET";       
        qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
        m_apHttpObj->setHost( m_apFileUrl->host() );
        m_apHttpObj->request( *( m_apHttpRequestHeader ));
        m_rssmState = GET_SECOND ;
    }
    else if( m_rssmState == GET_SECOND )
    {
        QByteArray aa = m_apHttpObj->readAll();
        DebugUtils::DumpReponseToFile(aa,"get_second");
        QString newUrl = aa;
        newUrl = parseResponseAndGetNewUrl( newUrl );
        int iFileSize = parseResponseAndGetFileSize( aa ) ;
        m_pDownloadInfo->m_DownloadFileSize = iFileSize ; 
        if(newUrl.isEmpty() )
        {
            qDebug()<<("Could not find file on server");
            m_pDownloadInfo->m_State = DownloadState::FAILED;
            emit statusChanged( m_pDownloadInfo->m_State );
            return ;
        }
        setUrlFileAddress( newUrl );
        emit statusChanged( m_pDownloadInfo->m_State );
        m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
        m_DownloadServerHost = QUrl(newUrl).host();
        m_apHttpRequestHeader->setRequest("POST", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
        m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
        m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
        m_apHttpRequestHeader->setValue("Content-Type", "application/x-www-form-urlencoded");
        m_apHttpRequestHeader->setValue("Content-Length", "16");
        m_apHttpRequestHeader->setValue("Referrer", m_ReferrerFileAddress);
        qDebug() << "First post";       
        qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
        m_apHttpObj->setHost( m_apFileUrl->host() );
        m_apHttpObj->request( *( m_apHttpRequestHeader ), "dl.start=PREMIUM");
        m_rssmState = POST_FIRST ;
    }
    else if( m_rssmState == POST_FIRST)
    {
        QByteArray aa = m_apHttpObj->readAll() ;
        QString newUrlpath = parsePostReponseAndGetAddress( QString( aa ) );
        DebugUtils::DumpReponseToFile(aa,"post_first");
        
        setUrlFileAddress(newUrlpath);
        //FIXME:
        emit statusChanged( m_pDownloadInfo->m_State );
        m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
        m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
        m_apHttpRequestHeader->setValue("Cookie", m_apRSUser->ComposeCookie() );
        m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
        m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress);
        qDebug() << "First post"; 
        m_rssmState = GET_THIRD; 
        qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;       
        m_apHttpObj->setHost( m_apFileUrl->host() );
        m_apHttpObj->request( *( m_apHttpRequestHeader ) );
        m_timerId = startTimer(1000);
        m_readedBytes = 0;
    }
    else if( m_pDownloadInfo->m_State == DownloadState::DONE )
    {   
        m_apFile->close();
        renameFile();
        emit statusChanged( m_pDownloadInfo->m_State );
        killTimer(m_timerId);
    }
    else if( m_pDownloadInfo->m_State == DownloadState::PAUSED ) 
    {
        QByteArray aa = m_apHttpObj->readAll() ;
        QString newUrlpath = parsePostReponseAndGetAddress( QString( aa ) );
        DebugUtils::DumpReponseToFile(aa,"paused");
    }
 };

 /*
 //fixme:
 void QRapidshareDownload::abort()
 {
    RSDM_LOG_FUNC ;
    //FIXME:
    //m_rssmState = DownloadState::STOPPED;
    m_pDownloadInfo->m_State = DownloadState::STOPPED;
    m_apHttpObj->abort();
    // do not emit, cause it will be removed from list. 
 }
 */

 QString QRapidshareDownload::parseResponseAndGetNewUrl(const QString & resp)
 {
    RSDM_LOG_FUNC ;
    QString line;
    QString newUrl;
    int z = 0;
    z = resp.indexOf("form action");
    if(z < 0 )
    {
        qDebug() << "Could not localize substring!" << resp ;
        if( resp.indexOf("The file could not be found") >=0 )
        {
            
        }
        return "";
    }
    for(;;)
    {
        if( resp.at(z) == '\n')
            break;
        line +=resp.at(z++);
    }
    bool quot = false;
    bool write = false;
    for(z = 0;z< line.size();++z)
    {
        if( line.at(z) == '"' )
        {
            if( !quot )
            {
                write = true;
            }
            else
                break;
            quot = !quot;
        }
        if( write )
            newUrl += line.at(z);
    }
    newUrl.remove('"');
    return newUrl;
 }
 int QRapidshareDownload::parseResponseAndGetFileSize(const QString & resp)
 {
    RSDM_LOG_FUNC ;
    QString line;
    QString newUrl;
    int z = 0;
    z = resp.indexOf("downloadlink");
    if(z < 0 )
    {
        qDebug() <<"err: Hoho, response is zero!";
        return -1;
    }
    for(;;)
    {
        if( resp.at(z) == '\n')
            break;
        line +=resp.at(z++);
    }
    bool stick = false;
    for(z = 0;z< line.size();++z)
    {
        if( line.at(z) == '|' )
        {
            stick = true;
        }
        if( stick )
        {
            if( line.at(z) == 'K')
                break;
            newUrl += line.at(z);
        }
    }
    newUrl.remove(' ');
    newUrl.remove('|');
    bool ok;
    int ret = newUrl.toInt(&ok) * 1000 ;
    if( !ok )
        return -1;
    return ret;
 }
 QString QRapidshareDownload::parsePostReponseAndGetAddress( const QString & resp )
 {
    RSDM_LOG_FUNC ;
    QString line;
    QString newUrl;
    int z = 0;
    z = resp.indexOf("form name");
    if(z < 0 )
    {
        qDebug() << " err : Could not localize substring!";
        return "";
    }
    for(;;)
    {
        if( resp.at(z) == '\n')
            break;
        line +=resp.at(z++);
    }
    int iStartPos = line.indexOf("http://");
    if( iStartPos < 0 )
        return "";
    for(int i=iStartPos;i < line.size() ;++i)
    {
        if(line.at(i) =='"')
            break;
        newUrl +=line.at(i);
    }
    qDebug() << newUrl;
    return newUrl;
 }
 void QRapidshareDownload::setUser(const QRapidshareUser & rsUser)
 {
    RSDM_LOG_FUNC ;
    m_apRSUser.reset(new QRapidshareUser(rsUser));  
 }
 void QRapidshareDownload::setUser(const std::string& rsName,  const std::string& rsPass)
 {
    RSDM_LOG_FUNC ;
    setUser(QRapidshareUser(rsName.c_str(),rsPass.c_str()));
 }

 void QRapidshareDownload::timerEvent(QTimerEvent *event)
 {
    RSDM_LOG_FUNC ;
    emit downloadRate( QString("%1").arg( ((double) m_readedBytes / 1024),0, 'f',2) ); 
    m_readedBytes = 0;
 };
 const QString QRapidshareDownload::getFullUrlFileAddress() const
 {
    RSDM_LOG_FUNC ;
    return m_ReferrerFileAddress ; 
 }
 const QString QRapidshareDownload::getFileDestination() const
 {
    RSDM_LOG_FUNC ;
    return m_qFileDestination ;
 };
 
 QRapidshareUser * QRapidshareDownload::getUser() const
 {
    return m_apRSUser.get() ; 
 }

 const QString QRapidshareDownload::getDownloadHost() const
 {
    return m_DownloadServerHost; 
 }
 
 void QRapidshareDownload::setDownloadHost( const QString & _host )
 {
    m_DownloadServerHost = _host ;
 }
 
 void QRapidshareDownload::renameFile()
 {
    if(m_apFile.get()!=NULL && !m_qFileDestination.isEmpty())
        QFile::rename(m_apFile->fileName(), m_qFileDestination);
 }

 bool QRapidshareDownload::checkForErrors( const QByteArray& response )
 {
     QList<const char*>::iterator it = m_errorsList.begin();
     while( it != m_errorsList.end() )
     {
         if( response.contains(*it))
             return true;
         ++it;
     }
     return false;
 }
 
 QString QRapidshareDownload::transformUrlPathToLocalPath(const QString & url)
{
    RSDM_LOG_FUNC ;
    QString ret = QString(url);
    ret = ret.right(ret.length() - ret.lastIndexOf("/") - 1);
    int ind = ret.indexOf(".htm");
    if( ind != -1 )
    {
        ret = ret.left( ind );
    }
    return ret;
}


