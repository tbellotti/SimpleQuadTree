#!/usr/bin/env bash

for i in {0..19}
do
	echo "Compiling LaTeX $i"
	pdflatex "./media/movie$i.tex"
	echo "Converting to PNG $i"
	convert -density 150 "./media/movie$i.pdf" -quality 90 "./media/movie$i.png"
done

