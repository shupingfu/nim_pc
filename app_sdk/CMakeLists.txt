MESSAGE("############# app_sdk #############")

SET(TARGET_NAME app_sdk)

string(TIMESTAMP CMKE_BUILD_TIME "%Y-%m-%dT%H:%M:%SZ" UTC)

if(NOT kAppKey)
    message(FATAL_ERROR "Please define kAppKey")
endif()

if(NOT kAppServerAddress)
    message(FATAL_ERROR "Please define kAppServerAddress")
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/app_sdk.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/app_sdk.h")

PROJECT(${TARGET_NAME})

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/../
    ${PROJECT_SOURCE_DIR}/../tool_kits/
    ${PROJECT_SOURCE_DIR}/../include/
)

FILE(GLOB_RECURSE APP_SDK_SOURCE *.cc *.h *.cpp *.c)

ADD_LIBRARY(${TARGET_NAME} STATIC ${APP_SDK_SOURCE})