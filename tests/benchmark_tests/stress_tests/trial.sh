#!/bin/bash
ab -n 1 -c 1 -v 4 -p post_file.txt -T application/x-www-form-urlencoded  "http://vbox:8080/?fname=Mitchell&lname=Gouzenko"
