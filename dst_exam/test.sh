#!/bin/bash
find dir/ -name lab0_x > lab0_exam.c
grep -n "hello OS lab0" dir/*/* >> lab0_exam.c
