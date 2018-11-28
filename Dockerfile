FROM ubuntu:latest

LABEL MAINTAINER="Sharif Ahamd <sharifrah@gmail.com>"

RUN apt-get update && apt-get install -y \
        build-essential \
        cmake \
        git \
        wget \
        && wget -O code.deb https://az764295.vo.msecnd.net/stable/bc24f98b5f70467bc689abf41cc5550ca637088e/code_1.29.1-1542309157_amd64.deb \
        && dpkg -i code.deb; apt-get install -y -f \
        && rm code.deb

RUN apt-get update && apt-get install -y libasound2
RUN code --user-data-dir="~/.vscode-root" --install-extension ms-vscode.cpptools --force

RUN git clone https://github.com/abseil/googletest.git /src/gtest

WORKDIR /src/gtest

RUN mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . \
    && make install

CMD code --user-data-dir="~/.vscode-root" -w .
