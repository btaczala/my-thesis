CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(DownloadManager_lib)

find_package ( Qt4 REQUIRED )
find_package ( Boost REQUIRED ) 

add_definitions( -DENGINES_BUILT_IN )
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(QT_USE_QTNETWORK 1)
include_directories ( "src/" ) 
SET ( BUILD_TEST_APP ON ) 

SET( DownloadManager_SRCS 
    src/downloadmanager.cpp
    src/downloadengine.cpp
    src/idownload.cpp
    src/rslogger.cpp 
    src/rscommon.cpp 
    src/enginemanager.cpp
    src/optionscontainer.cpp
    src/engines/rapidshare/qrapidsharedownload.cpp
    src/engines/rapidshare/qrapidshareuser.cpp
    src/engines/http/httpdownload.cpp
    src/engines/ftp/ftpdownload.cpp
	
) 
SET( DownloadManager_HDRS
    src/downloadmanager.h
    src/downloadengine.h
    src/idownload.h
    src/rslogger.h 
    src/rscommon.h
    src/enginemanager.h
    src/engine.h
    src/engine.inl
    src/optionscontainer.h
    src/progressinfo.h
    src/engines/rapidshare/qrapidsharedownload.h
    src/engines/rapidshare/qrapidshareuser.h
    src/engines/http/httpdownload.h
    src/engines/ftp/ftpdownload.h
)
SET ( DownloadManager_MOC_HDRS 
    src/idownload.h
    src/downloadmanager.h
)
SET ( PLUGINS_MOC_HDRS 
    src/engines/rapidshare/qrapidsharedownload.h
    src/engines/http/httpdownload.h
    src/engines/ftp/ftpdownload.h
)

QT4_WRAP_CPP(QT_MOC_SRC ${DownloadManager_MOC_HDRS} ${PLUGINS_MOC_HDRS} )
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES( "../proxy_lib/src" )
ADD_LIBRARY( downloadmanager_lib STATIC ${QT_MOC_SRC} ${DownloadManager_SRCS} ${DownloadManager_HDRS} )
target_link_libraries ( downloadmanager_lib ${QT_LIBRARIES}  ) 

if( BUILD_TEST_APP )
QT4_WRAP_CPP(QTAPP_MOC_SRC qtestapp.h )
ADD_EXECUTABLE(qrsdmtest qrsdmtest.cpp ${QTAPP_MOC_SRC} )
TARGET_LINK_LIBRARIES(qrsdmtest downloadmanager_lib ${QT_LIBRARIES} proxy_lib)
endif( BUILD_TEST_APP )
