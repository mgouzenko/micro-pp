#!/bin/bash
set -e
cd ./unit_test_apps
make
./unit_test_app > /dev/null &
cd .. 
python test_response.py
pkill -2 test_app
