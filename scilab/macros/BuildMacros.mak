
all:: algebre arma auto calpol elem int metanet mtlb optim percent robust sci2for signal sound statistics tdcs texmacs tksci util xdess scicos scicos_blocks m2sci sparse HighLevelPlotting

algebre	:
	@cd algebre
	@echo Creation of algebre (Macros)
	@makemacros > nul
	@cd..

arma	:	
	@cd arma
	@echo Creation of arma (Macros)
	@makemacros > nul
	@cd..
	
auto	:	
	@cd auto
	@echo Creation of auto (Macros)
	@makemacros > nul
	@cd..

calpol	:	
	@cd calpol
	@echo Creation of calpol (Macros)
	@makemacros > nul
	@cd..

elem	:	
	@cd elem
	@echo Creation of elem (Macros)
	@makemacros > nul
	@cd..
	
int	:	
	@cd int
	@echo Creation of int (Macros)
	@makemacros > nul
	@cd..


metanet	:	
	@cd metanet
	@echo Creation of metanet (Macros)
	@makemacros > nul
	@cd..
	
mtlb	:	
	@cd mtlb
	@echo Creation of mtlb (Macros)
	@makemacros > nul
	@cd..

optim	:	
	@cd optim
	@echo Creation of optim (Macros)
	@makemacros > nul
	@cd..
	
percent	:	
	@cd percent
	@echo Creation of percent (Macros)
	@makemacros > nul
	@cd..
	
robust	:	
	@cd robust
	@echo Creation of robust (Macros)
	@makemacros > nul
	@cd..
	
sci2for	:	
	@cd sci2for
	@echo Creation of sci2for (Macros)
	@makemacros > nul
	@cd..
	
scicos	:	
	@cd scicos
	@echo Creation of scicos (Macros)
	@makemacros > nul
	@cd..
	
signal	:	
	@cd signal
	@echo Creation of signal (Macros)
	@makemacros > nul
	@cd..
	
sound	:	
	@cd sound
	@echo Creation of sound (Macros)
	@makemacros > nul
	@cd..
	
statistics	:	
	@cd statistics
	@echo Creation of statistics (Macros)
	@makemacros > nul
	@cd..

tdcs	:	
	@cd tdcs
	@echo Creation of tdcs (Macros)
	@makemacros > nul
	@cd..

texmacs	:	
	@cd texmacs
	@echo Creation of texmacs (Macros)
	@makemacros > nul
	@cd..
	
tksci	:	
	@cd tksci
	@echo Creation of tksci (Macros)
	@makemacros > nul
	@cd..

util	:	
	@cd util
	@echo Creation of util (Macros)
	@makemacros > nul
	@cd..
	
xdess	:	
	@cd xdess
	@echo Creation of xdess (Macros)
	@makemacros > nul
	@cd..

sparse:
	@cd sparse
	@echo Creation of sparse (Macros)
	@makemacros > nul
	@cd..

scicos_blocks	:
	@cd scicos_blocks
	@echo Creation of scicos_blocks (Macros)
	@buildmacros.bat > nul
	@cd..

m2sci	:	
	@cd m2sci
	@echo Creation of m2sci (Macros)
	@buildmacros.bat > nul
	@cd..
	
HighLevelPlotting:	
	cd HighLevelPlotting
	@echo Creation of HighLevelPlotting (Macros)
	@makemacros > nul
	@cd..
	
clean::
	-del *.bin /s
	-del names.* /s
	-del genlib /s
	-del *.moc /s
	