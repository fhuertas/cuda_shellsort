#!/usr/bin/make -f
MAINFILE = main
BIB=$(wildcard *.bib)
RELEASENAME=release-`date +%H%M%S-%d%m%y`.tgz

	

all:	figs pdf 

release: distclean
	@echo $(RELEASENAME)
	tar czvf $(RELEASENAME) *
	

pdf:	$(MAINFILE)
dvi:	$(MAINFILE).dvi
full: realclean figs all
$(MAINFILE):	$(MAINFILE).bbl
	@echo "----Reecompilando para las referencias----"
	pdflatex $(patsubst %.pdf,%,$@)

$(MAINFILE).dvi:	$(MAINFILE).bbl
	latex $(patsubst %.dvi,%,$@)

$(MAINFILE).bbl:	$(MAINFILE).aux
	@echo "----Incluyendo la biblioteca----"
	bibtex $(patsubst %.bbl,%,$@)
	pdflatex $(patsubst %.bbl,%,$@)

$(MAINFILE).aux:	$(MAINFILE).tex 
	@echo "----haciendo el aux----"
	pdflatex $(patsubst %.aux,%,$@)

$(MAINFILE).tex:
# Cosas a hacer antes del tex

$(BIB):
# Cosas que hacer con las bibliotecas



figs:
	cd Figs; make


CLEAN_DIST=$(wildcard  $(MAINFILE).dvi $(MAINFILE).aux $(MAINFILE).out \
		$(MAINFILE).bbl $(MAINFILE).blg $(MAINFILE).log $(MAINFILE).tex.backup $(MAINFILE).pdfsync \
		*.bak *~ *.backup *~*)

CLEANFILES=$(wildcard $(MAINFILE).pdf $(CLEAN_DIST))


clean: 
ifeq ("","$(CLEAN_DIST)")
	@echo Already clean
else	
	@rm -vf $(MAINFILE).pdf $(CLEAN_DIST)
	@cd Chapters; make clean
	@cd extra_tex; make clean
	@cd Apendices; make clean
	@cd packages; make clean
endif
distclean: realclean
ifeq ("","$(CLEAN_DIST)")
	@echo Already clean
else
	@rm -vf  $(CLEAN_DIST)
	@cd Chapters; make clean
	@cd extra_tex; make clean
	@cd Apendices; make clean
	@cd packages; make clean
	@cd Figs; make clean
endif

realclean:
	@cd Figs; make clean
	

