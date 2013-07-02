#!/bin/tcsh

#LIB_DIR = $1

tar -cvzf ${1}.tgz ${1}/*.itf ${1}/*.h ${1}/*.hh ${1}/*.c ${1}/*.cc ${1}/*.cpp ${1}/Makefile ${1}/*.pro ${1}/*.txt ${1}/*.ln ${1}/*.sh ${1}/*.ui

