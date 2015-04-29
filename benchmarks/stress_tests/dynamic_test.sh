#!/bin/bash
/usr/local/Cellar/ab/2.4.12/bin/ab -n 10000 -c 10 -T application/x-www-form-urlencoded -p post_file.txt "http://vbox:8080/?fname=Mitchell&lname=Gouzenko"
