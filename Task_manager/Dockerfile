FROM debian:stable

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    && rm -rf /var/lib/apt/lists/*

COPY . /app
WORKDIR /app

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/Task_manager"]
