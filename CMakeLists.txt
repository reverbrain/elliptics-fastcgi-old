cmake_minimum_required( VERSION 2.6 ) 
 
project(elliptics-fastcgi) 
 
FILE (READ "${CMAKE_CURRENT_SOURCE_DIR}/debian/changelog" DEBCHANGELOG)

string(REGEX MATCH "([0-9]+\\.[0-9]+-[0-9]+)" DEBFULLVERSION "${DEBCHANGELOG}")
STRING (REGEX MATCH "([0-9]+\\.[0-9]+)" MAJOR_VERSION "${DEBFULLVERSION}")
STRING (REGEX MATCH "([0-9]+$)" MINOR_VERSION "${DEBFULLVERSION}")

message("Deb full version: ${DEBFULLVERSION}")
 
option(WITH_METABASE "Build with metabase and metabalancer support" ON)
option(WITH_REGIONAL_MODULE "Build with regional module support" ON)

SET(CMAKE_CXX_FLAGS "-Wall -std=c++0x")

find_package(Boost COMPONENTS system thread REQUIRED)

set(REQUIRED_LIBRARIES
                      elliptics_cpp
                      curl
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_THREAD_LIBRARY})

SET(HAVE_METABASE false)
IF(WITH_METABASE)
    SET(HAVE_METABASE true)
    FIND_LIBRARY(DEALER NAMES cocaine-dealer)

    IF(${DEALER} STREQUAL "DEALER-NOTFOUND")
        SET(HAVE_METABASE false)
    ENDIF()

    IF(HAVE_METABASE)
        SET(REQUIRED_LIBRARIES 
			${REQUIRED_LIBRARIES}
			${DEALER})
    ENDIF()
message("Metabase: ${HAVE_METABASE} ${DEALER}")
ENDIF()

SET(HAVE_REGIONAL false)
IF(WITH_REGIONAL_MODULE)
    SET(HAVE_REGIONAL true)
    FIND_LIBRARY(REGIONAL_MODULE names elliptics-regional-module PATHS ${CMAKE_INSTALL_PREFIX}/lib/fastcgi2)

    IF(${REGIONAL_MODULE} STREQUAL "REGIONAL_MODULE-NOTFOUND")
        SET(HAVE_REGIONAL false)
    ENDIF()

    IF(HAVE_REGIONAL)
        SET(REQUIRED_LIBRARIES 
			${REQUIRED_LIBRARIES}
			${REGIONAL_MODULE})
    ENDIF()
message("Regional module: ${HAVE_REGIONAL} ${REGIONAL_MODULE}")
ENDIF()


CONFIGURE_FILE(
	"${PROJECT_SOURCE_DIR}/module/settings.h.in"
	"${PROJECT_SOURCE_DIR}/module/settings.h")
	
 
AUX_SOURCE_DIRECTORY(module SOURCES)

ADD_LIBRARY(elliptics-fastcgi SHARED ${SOURCES})
 
SET_TARGET_PROPERTIES(
    elliptics-fastcgi PROPERTIES
        PREFIX ""
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/fastcgi2"
        VERSION "${MAJOR_VERSION}.${MINOR_VERSION}"
    )
 
TARGET_LINK_LIBRARIES(elliptics-fastcgi
                      ${REQUIRED_LIBRARIES})

INSTALL(
    TARGETS
        elliptics-fastcgi
    LIBRARY DESTINATION lib/fastcgi2 COMPONENT runtime)

