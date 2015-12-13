#!/bin/bash

appName="Solver++"

cd Debug
make clean
make -B all
cd ../
if [ $? -eq 0 ]; then
	mv "./Debug/$appName" "./$appName"
	echo "Memchecking..."
	bash memcheck.bash "./$appName" "valgrind.log"
	echo "Done..."
fi
