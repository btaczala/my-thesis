CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(DownloadManager_lib)

find_package ( Qt4 REQUIRED )
find_package ( Boost REQUIRED ) 

add_definitions( -DENGINES_BUILT_IN )
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(QT_USE_QTNETWORK 1)
include_directories ( "src/" ) 

SET( DownloadManager_SRCS 
    src/downloadmanager.cpp
    src/downloadengine.cpp
    src/idownload.cpp
    src/rapidsharedownloadmanager.cpp 
    src/rslogger.cpp 
    src/rscommon.cpp 
    src/enginemanager.cpp
    src/engines/rapidshare/qrapidsharedownload.cpp
    src/engines/rapidshare/qrapidshareuser.cpp
    src/engines/rapidshare/rapidshareengine.cpp
) 
SET( DownloadManager_HDRS
    src/downloadmanager.h
    src/downloadengine.h
    src/idownload.h
    src/rapidsharedownloadmanager.h 
    src/rslogger.h 
    src/rscommon.h
    src/enginemanager.h
    src/engines/rapidshare/qrapidsharedownload.h
    src/engines/rapidshare/qrapidshareuser.h
    src/engines/rapidshare/rapidshareengine.h
)
#SUBDIRS(src)
QT4_WRAP_CPP(QT_MOC_SRC src/engines/rapidshare/qrapidsharedownload.h src/rapidsharedownloadmanager.h src/downloadmanager.h )
INCLUDE(${QT_USE_FILE})
include_directories(${Boost_INCLUDE_DIR})
add_library( downloadmanager_lib STATIC ${QT_MOC_SRC} ${DownloadManager_SRCS} ${DownloadManager_HDRS} )

if( BUILD_TEST_APP )
ADD_EXECUTABLE(qrsdmtest qrsdmtest.cpp)
TARGET_LINK_LIBRARIES(qrsdmtest downloadmanager_lib)
endif( BUILD_TEST_APP )