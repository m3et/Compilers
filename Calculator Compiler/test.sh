#!/bin/bash

> Test/out.txt
for i in {1..10}
do
	echo "~~ TEST NUMBER $i ~~" >> Test/out.txt
	./calc < Test/test$i.cal &>> Test/out.txt
done

diff -ys Test/out.txt Test/test.txt
