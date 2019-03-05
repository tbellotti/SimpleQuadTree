#!/usr/bin/env bash

for i in {0..19}; do
	echo "Compiling LaTeX $i"
	pdflatex "frame$i.tex"
	echo "Converting to PDF $i"
	convert -density 150 "frame$i.pdf" -quality 90 "frame$i.png"
done

