#!/bin/bash

for (( i=0; i<$1; ++i ))
do
	python gen.py $2
	./da6 < input > resС
	diff resС output

	python bench.py < input > resPy
	diff resPy output
done