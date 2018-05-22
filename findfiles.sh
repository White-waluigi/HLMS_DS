#!/bin/bash




stuff=$(find src/ds -iname '*.cpp')
final=$(printf "set(SOURCES_DS \n$stuff )")


stuff=$(find src/demo -iname '*.cpp')
final=$( printf "$final \n\nset(SOURCES_DEMO \n$stuff )")




echo "$final" > src/_CMakeLists.txt



diff --brief src/_CMakeLists.txt src/CMakeLists.txt >/dev/null


comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "integrating new files"
    cp src/_CMakeLists.txt src/CMakeLists.txt
else
    echo "no new files"
fi


echo "Files added to CMake"


