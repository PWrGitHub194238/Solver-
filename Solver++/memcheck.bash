#!/bin/bash

echo "Running valgrind (log file will be generated as '$2')...";
valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --show-reachable=yes --log-file=$2 --gen-suppressions=all -v $1
#
