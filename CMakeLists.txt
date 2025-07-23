cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED BUILD_TARGET)
    message(FATAL_ERROR "-D BUILD_TARGET was not defined")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_PATH ${BROWSER_LIB_DIR} ${CMAKE_LIBRARY_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/.cmake ${CMAKE_MODULE_PATH})
file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.cc)

include(util)

project(${PROJECT_NAME}
    VERSION
        0.0.0
    LANGUAGES
        CXX
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0)

add_executable(${PROJECT_NAME} ${BUILD_TARGET} ${SRCS})

set_browser_definition()
set_browser_linked_lib()

message(STATUS ${BROWSER_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${GTKMM_INCLUDE_DIRS}
    ${BROWSER_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS} ${BROWSER_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    -Wl,-Bstatic
        winpthread
    -Wl,-Bdynamic
    ${GTKMM_LIBRARIES}
    WebView2Loader)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES
    ${BROWSER_LIB_DIR}/${BROWSER_LINKED_LIB}
    DESTINATION bin
)

