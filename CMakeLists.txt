cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

include(FetchContent)

set(FETCHCONTENT_QUIET OFF CACHE INTERNAL "Override verbose output" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Override update strategy" FORCE)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

###############################################################################

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        pozitie.cpp
        pozitie.h
        celula.cpp
        celula.h
        labirint.cpp
        labirint.h
        jocdungeon.cpp
        jocdungeon.h
        entitate.cpp
        entitate.h
        jucator.cpp
        jucator.h
        inamic.cpp
        inamic.h
        obiect.cpp
        obiect.h
        potiune.cpp
        potiune.h
        inventar.cpp
        inventar.h
        arma.cpp
        arma.h
        sabie.cpp
        sabie.h
        pistoale.cpp
        pistoale.h
        spear.cpp
        spear.h
        gamedata.cpp
        gamedata.h
        battlelog.cpp
        battlelog.h
        radar.cpp
        radar.h
        exceptii.h
        fantoma.cpp
        fantoma.h
        vrajitorinamic.cpp
        vrajitorinamic.h
)
target_sources(${MAIN_EXECUTABLE_NAME}
        PRIVATE
        src/Example.cpp
)

set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################

target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
        ext
        ${sfml_SOURCE_DIR}/include
)

target_link_libraries(${MAIN_EXECUTABLE_NAME}
        sfml-graphics
        sfml-window
        sfml-system
)

###############################################################################

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})