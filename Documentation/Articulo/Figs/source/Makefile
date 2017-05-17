#!/usr/bin/make -f

#FIGS-EPS-PDF=$(patsubst %.eps,%.pdf,$(wildcard *.eps))
#FIGS-SVG-PDF=$(patsubst %.svg,%.pdf,$(wildcard *.svg))
#FIGS-DIA-PDF=$(patsubst %.dia,%.pdf,$(wildcard *.dia))
#FIGS-PNG-PDF=$(patsubst %.png,%.pdf,$(wildcard *.png))
#FIGS-GLP-PDF=$(patsubst %.glp,%.pdf,$(wildcard *.glp))

FIGS-SVG-EPS=$(patsubst %.svg,%.eps,$(wildcard *.svg))
FIGS-DIA-EPS=$(patsubst %.dia,%.eps,$(wildcard *.dia))
FIGS-PNG-EPS=$(patsubst %.png,%.eps,$(wildcard *.png))
FIGS-GLP-EPS=$(patsubst %.glp,%.eps,$(wildcard *.glp))

FIGS-EPS=$(FIGS-SVG-EPS) $(FIGS-DIA-EPS) $(FIGS-PNG-EPS) $(FIGS-GLP-EPS)
FIGS-PDF=$(patsubst %.eps, %.pdf, $(FIGS-EPS))

CLEANFILES=$(wildcard *~)

all:	figuras-eps figuras-pdf
#	mv *.eps ../eps/
#	mv *.pdf ../pdf/

figuras-eps: 
# Hacerlo desde el make en los directorios eps y pdf permite al make
# saber si ya está actualizado y no generar todos cada vez que se hace make
	cd ../eps/;make $(FIGS-EPS);

figuras-pdf:
	cd ../pdf/;make $(FIGS-PDF);
clean: 
	rm -vf $(CLEANFILES)


#figuras: $(FIGS-EPS-PDF) $(FIGS-SVG-PDF) $(FIGS-DIA-PDF) $(FIGS-PNG-PDF) $(FIGS-GLP-PDF) $(FIGS-SVG-EPS) $(FIGS-DIA-EPS) $(FIGS-PNG-EPS) $(FIGS-GLP-EPS)
#	
#
#
#%.pdf:	%.eps
#	epstopdf ../eps/$< --outfile=../pdf/$@
#
#%.eps:	%.gpl
#	gnuplot	$<
#
#%.eps:	%.svg
#	inkscape -E ../eps/$@ $<
	
#%.eps:  %.dia
#	dia $< -t eps -e ../eps/$@
#
#%.eps:  %.png
#	convert $< ../eps/$@ 

# CLEANFILES=$(wildcard (FIGS-SVG-PDF) $(FIGS-DIA-PDF) $(FIGS-PNG-PDF) $(FIGS-GLP-PDF) $(FIGS-SVG-EPS) $(FIGS-DIA-EPS) $(FIGS-PNG-EPS) $(FIGS-GLP-EPS) $(FIGS-EPS-PDF))

# clean: 
# ifeq ("","$(CLEANFILES)")
# 	@echo Archivos temporales borrados
# else
# 	@echo No limpiados
# 	rm -vf  $(CLEANFILES)
# endif

