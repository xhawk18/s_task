#!/bin/bash
mkdir -p "./build"
cd "./build"
cmake -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE=flash_xip -DBOARD=hpm6200evk ..
ninja
