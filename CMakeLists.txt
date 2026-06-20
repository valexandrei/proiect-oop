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

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.2
        GIT_SHALLOW    1
        GIT_PROGRESS   ON
)
FetchContent_MakeAvailable(SFML)

###############################################################################

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        src/pozitie.cpp
        src/celula.cpp
        src/labirint.cpp
        src/jocdungeon.cpp
        src/entitate.cpp
        src/jucator.cpp
        src/inamic.cpp
        src/obiect.cpp
        src/potiune.cpp
        src/inventar.cpp
        src/arma.cpp
        src/sabie.cpp
        src/pistoale.cpp
        src/spear.cpp
        src/gamedata.cpp
        src/battlelog.cpp
        src/radar.cpp
        src/fantoma.cpp
        src/vrajitorinamic.cpp
        src/schelet.cpp
        include/schelet.h
        include/pozitie.h
        include/celula.h
        include/labirint.h
        include/jocdungeon.h
        include/entitate.h
        include/jucator.h
        include/inamic.h
        include/obiect.h
        include/potiune.h
        include/inventar.h
        include/arma.h
        include/sabie.h
        include/pistoale.h
        include/spear.h
        include/gamedata.h
        include/battlelog.h
        include/radar.h
        include/exceptii.h
        include/fantoma.h
        include/vrajitorinamic.h
        include/renderer.h
        src/renderer.cpp
        include/gamelogic.h
        src/gamelogic.cpp
        include/inamicfactory.h
        src/inamicfactory.cpp
        include/stiva.h
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