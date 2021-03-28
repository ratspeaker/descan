cmake_minimum_required(VERSION 3.16)
project(Descan, VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#set(CMAKE_MODULE_PATH "/home/maja/Qt/5.15.1/gcc_64/lib/cmake/Qt5NetworkAuth")
#set(Qt5_DIR "/home/maja/Qt/5.15.1/gcc_64/lib/cmake/Qt5NetworkAuth" CACHE PATH "Initial cahe" FORCE)

find_package(Qt5 COMPONENTS Widgets NetworkAuth)

find_package(Qt5 COMPONENTS NetworkAuth)

find_package(CURL)

find_library(LIB_TO_INCLUDE PDFNetC ${CMAKE_SOURCE_DIR}/PDFNetC64/Lib)

include_directories(${CMAKE_SOURCE_DIR}/PDFNetC64/Headers)

if (EXISTS ${CMAKE_SOURCE_DIR}/PDFNetC64/Headers)
    set(PDFNetC_Include_Dir ${CMAKE_SOURCE_DIR}/PDFNetC64/Headers CACHE PATH "Path to the PDFNetC header files.")
    message("HEADER DIR: ${PDFNetC_Include_Dir}")
else ()
    message(FATAL_ERROR "PDFNetC headers directory not found. Cannot continue.")
endif ()

if (EXISTS ../PDFNetC64/Lib/libPDFNetC.so)
    set(PDFNetC_Library ../PDFNetC64/Lib/libPDFNetC.so CACHE FILEPATH "File path to the PDFNetC library.")
    message("${PDFNetC_Library}")
    
endif ()


set(SOURCES_PATH "../Descan/sources")
set(HEADERS_PATH "../Descan/headers")
set(FORMS_PATH "../Descan/forms")
set(CMAKE_AUTOUIC_SEARCH_PATHS ../07-descan/Descan/forms)

set(SOURCES ${SOURCES_PATH}/main.cpp ${SOURCES_PATH}/mainwindow.cpp ${SOURCES_PATH}/image.cpp ${SOURCES_PATH}/dialogmail.cpp ${SOURCES_PATH}/displayarea.cpp ${SOURCES_PATH}/drive.cpp ${SOURCES_PATH}/pdfhandler.cpp)
set(HEADERS ${HEADERS_PATH}/mainwindow.h ${HEADERS_PATH}/image.h ${HEADERS_PATH}/dialogmail.h ${HEADERS_PATH}/displayarea.h ${HEADERS_PATH}/drive.h ${HEADERS_PATH}/pdfhandler.h)
set(UI ${FORMS_PATH}/mainwindow.ui ${FORMS_PATH}/dialogmail.ui)


add_executable(Descan ${SOURCES} ${HEADERS} ${UI})
qt5_use_modules(Descan Widgets Core Gui Network PrintSupport)

target_link_libraries(Descan PDFNetC Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network Qt5::PrintSupport Qt5::NetworkAuth ${QT_LIBRARIES})
