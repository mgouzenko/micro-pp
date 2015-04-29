#!/bin/bash
ab -n 10000 -c 100 "http://localhost:8080/?fname=Mitchell&lname=Gouzenko"
