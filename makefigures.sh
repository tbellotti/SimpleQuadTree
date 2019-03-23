#!/usr/bin/env bash

# THIS MUST BE UPDATE ACCORDING TO WHAT WE WANT
# TO COMPILE...

pdflatex -output-directory=media "./media/mandelbrot_refinement.tex"
pdflatex -output-directory=media "./media/mandelbrot_refinement_ctr.tex"
pdflatex -output-directory=media "./media/simple_refinement.tex"

