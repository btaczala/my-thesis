find_package(Qt4 REQUIRED )
if(WIN32)
ADD_DEFINITIONS(  )
else(WIN32)
ADD_DEFINITIONS( -Wall -O2 )
endif(WIN32)
set(QT_USE_QTNETWORK 1)

SET( SRCS 
    qrapidsharedownload.cpp 
    rapidsharedownloadmanager.cpp 
    qrapidshareuser.cpp 
    rslogger.cpp 
    rscommon.cpp ) 
SET( HDRS 
    qrapidsharedownload.h 
    qrapidshareuser.h 
    rapidsharedownloadmanager.h 
    rslogger.h 
    rscommon.h )

QT4_WRAP_CPP(QT_MOC_SRC qrapidsharedownload.h rapidsharedownloadmanager.h )
INCLUDE(${QT_USE_FILE})
add_library( QtRapidshareDownloadManager_corelib STATIC ${QT_MOC_SRC} ${SRCS} ${HDRS} )
target_link_libraries( QtRapidshareDownloadManager_corelib ${QT_LIBRARIES} )
#add definitions, compiler switches, etc.


#add an install target here
#INSTALL_FILES(...)
#INSTALL_PROGRAMS(...)
#INSTALL_TARGET(...)

