PROJECT(qrsdm)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

find_package(Qt4 REQUIRED )
find_package( Boost REQUIRED )
#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(QT_USE_QTNETWORK 1)

SUBDIRS(DownloadManager_lib)
SUBDIRS(QtRapidshareDownloadManager_app)
INCLUDE(${QT_USE_FILE})
include_directories(QtRapidshareDownloadManager_corelib)
include_directories(${Boost_INCLUDE_DIR})

#ADD_EXECUTABLE(qrsdmtest qrsdmtest.cpp)

#need to link to some other libraries ? just add them here
#TARGET_LINK_LIBRARIES(qrsdmtest RapidshareDownloadManager ${QT_LIBRARIES} )