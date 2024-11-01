# Укажите минимальную версию CMake
cmake_minimum_required(VERSION 3.5)
project(Server)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем Qt-модули
find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql REQUIRED)

# Указываем пути к библиотекам и заголовочным файлам PostgreSQL для Linux
find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

# Список исходных файлов
set(SRCS
    main.cpp
    server.cpp
    server.h
)

# Создание исполняемого файла
add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

# Линковка с библиотеками Qt и PostgreSQL
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Sql ${PostgreSQL_LIBRARIES})
