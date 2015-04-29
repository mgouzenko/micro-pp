#!/bin/bash
/usr/local/Cellar/ab/2.4.12/bin/ab -r -n 10000 -c 10 "http://vbox:8080/static_test.jpg"
