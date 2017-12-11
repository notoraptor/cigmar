#!/usr/bin/env bash
if test -e $HOME/.cache/cmake ; then
    echo "cmake already installed."
    $HOME/.cache/cmake --version
    $HOME/.cache/cmake/usr/local/bin/cmake --version
else
    echo "Installing cmake."
    sudo apt-get purge cmake
    rm -rf $HOME/.cache/cmake
    mkdir -p $HOME/.cache/tmp
    cd $HOME/.cache/tmp
    wget https://cmake.org/files/v3.9/cmake-3.9.4.tar.gz
    tar -xzf cmake-3.9.4.tar.gz
    cd cmake-3.9.4/
    ./bootstrap
    make -j4
    make DESTDIR=$HOME/.cache/cmake install
    echo "cmake installed."
fi
