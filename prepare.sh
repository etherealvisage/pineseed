#!/bin/sh

if [[ -d libs ]]; then
    echo "Already prepared!"
fi

mkdir libs

wget http://www.lua.org/ftp/lua-5.3.1.tar.gz -O libs/lua.tar.gz
pushd libs
tar xf lua.tar.gz
rm lua.tar.gz
make -C lua-5.3.1 linux
ln -s lua-5.3.1/src lua-include
ln -s lua-5.3.1/src/liblua.a
popd
