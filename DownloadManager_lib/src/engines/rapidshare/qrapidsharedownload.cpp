#include <QDebug>
#include <boost/any.hpp>

#include "qrapidsharedownload.h"
#include "optionscontainer.h"
#include "rscommon.h"

// FIXME:
#include <proxy.h>
#include <settings.h>

QRapidshareDownload::QRapidshareDownload(OptionsContainer* options): IDownload(options)
, m_apHttpObj( new QHttp() )
, m_apHttpRequestHeader(new QHttpRequestHeader() )
, m_apFileUrl( new QUrl() )
, m_iPDownloaded(0)
{
    LOG_AND_PROFILE;

    m_errorsList.append(RsErrors::err2);
    m_errorsList.append(RsErrors::err5);

    int content_length = Proxy::settings()->value(SettingsValNames::scContentLength,Settings::LIBRARY).value<int>() ; 
    if ( content_length == 0 ) 
    {
        Proxy::settings()->setValue( SettingsValNames::scContentLength ,scDefaultContentValue,Settings::LIBRARY);
    }
}
void QRapidshareDownload::connectToQHttp() {
    LOG_AND_PROFILE;
    connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
    connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
    connect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
    connect( m_apHttpObj.get(), SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
}
void QRapidshareDownload::disconnectFromQHttp() {
    LOG_AND_PROFILE;
    disconnect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
    disconnect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
    disconnect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
    disconnect( m_apHttpObj.get(), SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
}
void QRapidshareDownload::setFailed() {
    LOG_AND_PROFILE;
    disconnectFromQHttp();
    setState( DownloadState::FAILED,true );
}
void QRapidshareDownload::setStopped() {
    LOG_AND_PROFILE;
    disconnectFromQHttp();
    setState( DownloadState::STOPPED,true );
}
void QRapidshareDownload::setPaused() {
    LOG_AND_PROFILE;
    disconnectFromQHttp();
    setState( DownloadState::PAUSED,true );
}
QRapidshareDownload::~QRapidshareDownload(){
    LOG_AND_PROFILE;
    m_apHttpObj.get()->disconnect();
}

void QRapidshareDownload::start(){
    LOG_AND_PROFILE;
    setUrlFileAddress( m_UrlAddress.c_str() );
    //invalid url set
    if( m_apFileUrl->isEmpty())
    {
        setError("Ivalid url was passed to rapidshare engine");
        return;
    }
    m_ReferrerFileAddress = m_UrlAddress.c_str();
    
    initFile();
    removeFromFile(".htm");
    removeFromFile(".html");
    connectToQHttp();
    m_rssmState = GET_FIRST;
    setState( DownloadState::INIT, true );
    m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
    m_apHttpRequestHeader->setValue("Host",  m_apFileUrl->host() );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("Cookie", QRapidshareUser::ComposeCookie() );
    m_apHttpRequestHeader->setValue("User-Agent", "User-Agent: Opera/9.62 (Windows NT 5.1; U; pl) Presto/2.1.1");
    //m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress );
    m_apHttpRequestHeader->setValue("User-Accept", "application/xhtml+voice+xml;version=1.2, application/x-xhtml+voice+xml;version=1.2, text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1");
    qDebug() << QString("First GET");
    qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
    m_apHttpObj->setHost(  m_apFileUrl->host() );
    m_apHttpObj->request( *( m_apHttpRequestHeader ) );
}

void QRapidshareDownload::stop(){
    LOG_AND_PROFILE;
    disconnectFromQHttp();
    DownloadState::States curState = state();
    if( curState == DownloadState::STOPPED
        || curState  == DownloadState::DONE
        ||curState  == DownloadState::FAILED)
            return ; 
    
    setPaused();
    m_apHttpObj->abort();
}

void QRapidshareDownload::restart(){
    LOG_AND_PROFILE;
    connectToQHttp();
    m_iPDownloaded = downloadedBytes();
//     RSDM_LOG_FUNC ;
    //m_apHttpRequestHeader->removeValue(); // LENGTH REQUIRED ?? ;(
    m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
    m_apHttpRequestHeader->setValue("User-Agent", "User-Agent: Opera/9.62 (Windows NT 5.1; U; pl) Presto/2.1.1");
    m_apHttpRequestHeader->setValue("Host", m_DownloadServerHost );
    m_apHttpRequestHeader->setValue("User-Accept", "application/xhtml+voice+xml;version=1.2, application/x-xhtml+voice+xml;version=1.2, text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1");
    
    m_apHttpRequestHeader->setValue("Cookie", QRapidshareUser::ComposeCookie() );
    m_apHttpRequestHeader->setValue("Range", "bytes=" + QString::number(downloadedBytes())+ "-" );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress);
    int content_length = Proxy::settings()->value( SettingsValNames::scContentLength,Settings::LIBRARY).value<int>();
    if ( content_length == 0 ) 
        content_length = scDefaultContentValue ; 
    m_apHttpRequestHeader->setValue("Content-Length", QString::number( content_length ) );
    qDebug() << QString("Resumed !!!!!!");
    QString httpHeader = DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
    qDebug() << httpHeader;
    QString host = m_apFileUrl->host();
    m_apHttpObj->setHost( m_DownloadServerHost );
    m_apHttpObj->request( *( m_apHttpRequestHeader ) );
}

void QRapidshareDownload::setUrlFileAddress(const QString & _addr ){
    LOG_AND_PROFILE;
    DebugUtils::q_Log( QString(" _addr = ") + _addr);
    if( ! _addr.isEmpty() )
    {
        m_apFileUrl.reset( new QUrl( _addr ) );
    }
}

/********** SLOTS **************/

void QRapidshareDownload::requestFinished(const int & idReq, const bool & isFalse){  
    LOG_AND_PROFILE;
    closeFile();
    if( isFalse )
    {
        qDebug() << m_apHttpObj->errorString() ;
        return ; 
    }
}

void QRapidshareDownload::dataReadProgress(const int & done, const int & total){
    LOG_AND_PROFILE;
    //LOG( "done="<<done<< "total="<< total );
    // support for Direct downloads
    char *buff = NULL ; 
    qint64 iBytes2 = 0;
    qint64 bytes = m_apHttpObj->bytesAvailable();
    if( m_rssmState == GET_THIRD || m_rssmState == POST_FIRST )
    {
        buff = new char[bytes];
        iBytes2 = m_apHttpObj->read(buff, bytes);
        if( -1 == iBytes2 )
            LOG( "ERROR while reading from Http stream ");
        QString decive ( buff ) ;
        if( decive.contains("<!DOCTYPE") )
        {
            // this is still html. just do nothing 
            return ; 
        }
        if ( decive.contains("<h1>Error</h1>" ) )
        {
//             setState( DownloadState::FAILED, true );
            setFailed();
            setError( " Probably Password or Username are not correct " );
            m_apHttpObj->abort();
            return ; 
        }
        m_rssmState = DOWNLOADING;
        setState( DownloadState::DOWNLOADING, true ); 
    }
    if( state() == DownloadState::PAUSED )
    {
        if ( !openFile() )
        {
            setError("UnSuccessfull opening file");
            return ; 
        }
        setState( DownloadState::DOWNLOADING, true );
    }
    if ( state()  == DownloadState::DOWNLOADING ) 
    {
      calculateProgress( done + m_iPDownloaded, total + m_iPDownloaded);

        if ( buff == NULL ) 
        {
            buff = new char[bytes];
            iBytes2 = m_apHttpObj->read(buff, bytes);
        }
        if ( -1 == iBytes2)
            setError("ERROR while reading from Http stream");
        else
        {
            if( ! openFile() )
            {
                setError("unable to open file");
                setFailed();
                return ;
            }
            qint64 btmp = writeToFile(buff,iBytes2); 
            if( -1 == btmp )
            {
                qDebug()<<("write failed");
                m_apHttpObj->abort();
                setError("Unable to save downloaded data");
                setFailed();
                return;
            }
        }
        delete[] buff;
    }

    if( done + m_iPDownloaded == total + m_iPDownloaded &&  m_rssmState ==  DOWNLOADING )
    {
        setState( DownloadState::DONE );
        m_rssmState = FINISHED;
    }
}


void QRapidshareDownload::responseHeaderReceived( const QHttpResponseHeader & resp){
    LOG_AND_PROFILE;
    qDebug() << resp.reasonPhrase() ;
    int iStatusCode = resp.statusCode(); 
    if( iStatusCode == 200 || iStatusCode == 301 || iStatusCode == 302 || iStatusCode == 303 || iStatusCode == 307 )
    {
        QString newUrl = resp.value("Location");
        if ( !newUrl.isEmpty() )
        {

            //setUrlFileAddress( newUrl );
            m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
            m_DownloadServerHost = QUrl(newUrl).host();

            m_apHttpRequestHeader->setRequest("GET",QUrl(newUrl).path() );
            m_apHttpRequestHeader->setValue("User-Agent","Opera/9.62 (Windows NT 5.1; U; pl) Presto/2.1.1");
            m_apHttpRequestHeader->setValue("Host",m_DownloadServerHost);
            m_apHttpRequestHeader->setValue("Accept","Accept: application/xhtml+voice+xml;version=1.2, application/x-xhtml+voice+xml;version=1.2, text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1");
            m_apHttpRequestHeader->setValue("Cookie",QRapidshareUser::ComposeCookie());
            m_apHttpRequestHeader->setValue("Connection","Keep-Alive");
            m_apHttpObj->setHost( m_DownloadServerHost );
            m_apHttpObj->request( *( m_apHttpRequestHeader ));
            setState( DownloadState::DOWNLOADING , true );
            m_rssmState = DOWNLOADING;
            qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;
            LOG(QString("Starting download now %1!").arg(newUrl));
        }
        if ( m_apFileUrl.get() ) 
        {
            QString host = m_apFileUrl->host() ; 
            QRegExp regExp("^rs\\d+\\w+.rapidshare.com");
            if ( regExp.indexIn(host) != -1 ) 
            {
                if ( m_rssmState == GET_FIRST ) 
                {
                    LOG("Problably inserted address like http://rs666.rapidshare.com, which lead to direct download");
                    m_rssmState = DOWNLOADING;
                    setState( DownloadState::DOWNLOADING , true );
                }
            }
        }
    }
    else
        LOG("Error response:"<< iStatusCode);
}
void QRapidshareDownload::done(const bool & error){
    LOG_AND_PROFILE;
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
            setFailed();
            return;
        }

        DebugUtils::DumpReponseToFile(aa,"get_first");
        m_apHttpRequestHeader->removeValue("Cookie");
        m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Cookie", QRapidshareUser::ComposeCookie() );
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
        setTotalBytes( iFileSize );
        if(newUrl.isEmpty() )
        {
            qDebug()<<("Could not find file on server");
            setFailed();
            return ;
        }
        setUrlFileAddress( newUrl );
       
         //fixme: status changed here ?
        emitStatusChanged();
        
        m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
        m_DownloadServerHost = QUrl(newUrl).host();
        m_apHttpRequestHeader->setRequest("POST", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
        m_apHttpRequestHeader->setValue("Cookie", QRapidshareUser::ComposeCookie() );
        m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
        m_apHttpRequestHeader->setValue("Content-Type", "application/x-www-form-urlencoded");
        int content_length = Proxy::settings()->value( SettingsValNames::scContentLength,Settings::LIBRARY ).value<int>();
        if ( content_length == 0 ) 
            content_length = scDefaultContentValue ; 
        m_apHttpRequestHeader->setValue("Content-Length", QString::number( content_length ) );
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
        emitStatusChanged();
        m_apHttpRequestHeader.reset(new QHttpRequestHeader() );
        m_apHttpRequestHeader->setRequest("GET", m_apFileUrl->path() );
        m_apHttpRequestHeader->setValue("Host", m_apFileUrl->host() );
        m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
        m_apHttpRequestHeader->setValue("Cookie", QRapidshareUser::ComposeCookie());
        m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; Synapse)");
        m_apHttpRequestHeader->setValue("Referer", m_ReferrerFileAddress);
        qDebug() << "First post"; 
        m_rssmState = GET_THIRD; 
        qDebug() << DebugUtils::httpReqToString(*m_apHttpRequestHeader) ;       
        m_apHttpObj->setHost( m_apFileUrl->host() );
        m_apHttpObj->request( *( m_apHttpRequestHeader ) );
    }
    else if( state() == DownloadState::DONE )
    {   
        emitStatusChanged();
        disconnectFromQHttp();
    }
    else if( state() == DownloadState::PAUSED ) 
    {
        QByteArray aa = m_apHttpObj->readAll() ;
        QString newUrlpath = parsePostReponseAndGetAddress( QString( aa ) );
        DebugUtils::DumpReponseToFile(aa,"paused");
    }
};

QString QRapidshareDownload::parseResponseAndGetNewUrl(const QString & resp){
    LOG_AND_PROFILE;
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

qint64 QRapidshareDownload::parseResponseAndGetFileSize(const QString & resp){
    LOG_AND_PROFILE;
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
    qint64 ret = newUrl.toUInt(&ok) * 1000 ;
    if( !ok )
        return -1;
    return ret;
}
QString QRapidshareDownload::parsePostReponseAndGetAddress( const QString & resp ){
    LOG_AND_PROFILE;
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

const QString QRapidshareDownload::getFullUrlFileAddress() const{
    LOG_AND_PROFILE;
    return m_ReferrerFileAddress ; 
}

const QString QRapidshareDownload::getDownloadHost() const{
    LOG_AND_PROFILE;
    return m_DownloadServerHost; 
}

void QRapidshareDownload::setDownloadHost( const QString & _host ){
    LOG_AND_PROFILE;
    m_DownloadServerHost = _host ;
}
bool QRapidshareDownload::checkForErrors( const QByteArray& response ){
    LOG_AND_PROFILE;
    QList<const char*>::iterator it = m_errorsList.begin();
    while( it != m_errorsList.end() )
    {
        if( response.contains(*it))
        {
//             LOG(response);
            setError(*it);
            return true;
        }
        ++it;
    }
    return false;
}
