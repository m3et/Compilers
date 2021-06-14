#!/bin/bash
set -e
make
> out.txt
for i in {1..5}
do

	echo "~~ TEST NUMBER $i ~~" >> out.txt
	./a.out Test/test$i.txt >> out.txt
done

diff -y out.txt Test/test.txt