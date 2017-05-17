#!/usr/bin/make -f

FIGS-PDF=$(wildcard *.pdf)

CLEANFILES=$(wildcard $(FIGS-EPS) *~)

%.pdf:	../eps/%.eps
	epstopdf ../eps/$< --outfile=$@

clean: 
ifeq ("","$(CLEANFILES)")
	@echo Archivos temporales borrados
else
	@echo No limpiados
	rm -vf  $(CLEANFILES)
endif

