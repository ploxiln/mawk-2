
DOT_W = $(shell cd ..; ls *.w)
DVI_FILES = $(DOT_W:.w=.dvi)
PDF_FILES = $(DOT_W:.w=.pdf)
PS_FILES = $(DOT_W:.w=.ps)

doc : dvi pdf
	
dvi : $(DVI_FILES)

pdf : $(PDF_FILES)

ps : $(PS_FILES)

%.dvi : ../%.w
	mweave $< > $*.tex
	mwtex $*.tex
	rm -f $*.tex *.log *.cn? *.toc 

%.ps : %.dvi
	dvips -o $*.ps $<

%.pdf : %.dvi
	dvipdfm $<
	
clean :
	rm -f *.dvi *.ps *.tex *.cn? *.toc *.mpx *.pdf

