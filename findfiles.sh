#!/bin/bash




stuff=$(find src/ds -iname '*.cpp')
final=$(printf "set(SOURCES_DS \n$stuff )")


stuff=$(find src/demo -iname '*.cpp')
final=$( printf "$final \n\nset(SOURCES_DEMO \n$stuff )")



echo "$final" > src/CMakeLists.txt
echo "Files added to CMake"
