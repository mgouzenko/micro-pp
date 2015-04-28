#!/bin/bash
ab -n 1000 -c 10 -C color=blue http://localhost:8080/?fname=Mitchell&lname=Gouzenko
