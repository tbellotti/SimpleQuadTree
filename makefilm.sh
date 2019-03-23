#!/usr/bin/env bash

for i in {0..19}
do
	echo "Compiling LaTeX $i"
	pdflatex "movie$i.tex"
	echo "Converting to PNG $i"
	convert -density 150 "movie$i.pdf" -quality 90 "movie$i.png"
done

