#!/bin/bash
set -e
make
for i in {1..7}
do
	echo "~~ TEST NUMBER $i ~~"
	./a.out -t test/test$i.c
done
for i in {8..10}
do
	echo "~~ TEST NUMBER $i ~~"
	./a.out -x test/test$i.c
done
