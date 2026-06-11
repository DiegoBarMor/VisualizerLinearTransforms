#!/usr/bin/bash
set -eu

fbuild="build"

if [ ! -d "almond" ]; then
    git clone --depth 1 --branch source-only https://github.com/DiegoBarMor/almond tmp-almond
    mv tmp-almond/almond .
    rm -rf tmp-almond
fi

cmake -B $fbuild
cmake --build $fbuild
