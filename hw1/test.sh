#!/bin/bash
echo "Test string 1
Test string  1
hello world
CS 531
George Mason University
abcdefg hijk
George Mason University
k j i
Test string 2
test string *
test string 1
test String 1
D" | $1  | grep -v "Enter string" | grep -v "duplicate"