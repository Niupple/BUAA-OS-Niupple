#!/bin/bash
if [ -e $2 ]; then
	rm $2
fi
for i in 8 32 128 512 1024
do
	sed -n ${i}p $1 >> $2
done
