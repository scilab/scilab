LATEX = latex

RM = rm

PDFLATEX = pdflatex

MAKEINDEX = makeindex

doc :
	echo "Generating HTML and TEX files..."
	echo "exec('makedoc.sce');quit;" | scilab -nw
	echo "Compiling TEX files..."
	$(LATEX) M2SCI.tex
	$(PDFLATEX) M2SCI.tex
	$(LATEX) SCI2M.tex
	$(PDFLATEX) SCI2M.tex
	echo "Generating TEX index files..."
	$(MAKEINDEX) M2SCI.idx
	$(MAKEINDEX) SCI2M.idx
	echo "Re-Compiling TEX files..."
	$(LATEX) M2SCI.tex
	$(PDFLATEX) M2SCI.tex
	$(LATEX) SCI2M.tex
	$(PDFLATEX) SCI2M.tex

clean :
	$(RM) -f M2SCI.*
	$(RM) -f SCI2M.*
	$(RM) -f ./html/M2SCI.htm
	$(RM) -f ./html/SCI2M.htm
	$(RM) -f ./html/*_index.htm