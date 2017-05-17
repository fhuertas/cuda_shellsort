#!/usr/bin/make -f

FIGS-EPS=$(wildcard *.eps)

CLEANFILES=$(wildcard $(FIGS-EPS) *~)

%.eps:  ../source/%.dia
	dia ../source/$< -t eps -e $@

%.eps:	../source/%.gpl
	gnuplot	../source/$<

%.eps:	../source/%.svg
	inkscape -E $@ ../source/$<
	
%.eps:  ../source/%.png
	convert ../source/$< $@ 

clean: 
ifeq ("","$(CLEANFILES)")
	@echo Archivos temporales borrados
else
	@echo No limpiados
	rm -vf  $(CLEANFILES)
endif



