#!/bin/bash

for file in $(ls ./Codes/* | sort)
do
	root -b -q "$file"
done
