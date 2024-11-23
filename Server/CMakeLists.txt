# Укажите минимальную версию CMake
cmake_minimum_required(VERSION 3.5)
project(Server)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
<<<<<<< HEAD

# Изменено: добавлен модуль Sql
find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql REQUIRED)
=======
>>>>>>> de6eec12417335b64cafc028f5d82ba561aeb9fa

# Подключаем Qt-модули
find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql REQUIRED)

<<<<<<< HEAD
=======
# Указываем пути к библиотекам и заголовочным файлам PostgreSQL для Linux
find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

# Список исходных файлов
>>>>>>> de6eec12417335b64cafc028f5d82ba561aeb9fa
set(SRCS
    main.cpp
    server.cpp
    server.h
)

<<<<<<< HEAD
add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Sql)
=======
# Создание исполняемого файла
add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

# Линковка с библиотеками Qt и PostgreSQL
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Sql ${PostgreSQL_LIBRARIES})
>>>>>>> de6eec12417335b64cafc028f5d82ba561aeb9fa
