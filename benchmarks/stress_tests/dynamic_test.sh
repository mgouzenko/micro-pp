#!/bin/bash
ab -n 1 -c 1 -v 4 -T application/x-www-form-urlencoded -p post_file.txt "http://localhost:8080/?fname=Mitchell&lname=Gouzenko"
