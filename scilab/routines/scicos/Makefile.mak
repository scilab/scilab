#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/scicos.lib

BLOCKSF=evtdly.obj cstblk.obj \
	lusat.obj pload.obj qzcel.obj qzflr.obj\
	qzrnd.obj qztrn.obj scope.obj lsplit.obj csslti.obj\
	dsslti.obj trash.obj zcross.obj \
	expblk.obj logblk.obj sinblk.obj tanblk.obj powblk.obj\
	sqrblk.obj delay.obj selblk.obj forblk.obj writef.obj invblk.obj hltblk.obj\
	gensin.obj rndblk.obj lookup.obj timblk.obj gensqr.obj mfclck.obj\
	sawtth.obj tcslti.obj tcsltj.obj scopxy.obj evscpe.obj integr.obj readf.obj affich.obj\
	intpol.obj intplt.obj minblk.obj maxblk.obj dlradp.obj  iocopy.obj \
	sum2.obj sum3.obj delayv.obj mux.obj demux.obj samphold.obj dollar.obj mscope.obj \
	intrp2.obj   intrpl.obj fsv.obj memo.obj fscope.obj scoxy.obj diffblk.obj constraint.obj \
	absblk.obj andlog.obj bidon.obj gain.obj cdummy.obj dband.obj cosblk.obj ifthel.obj eselect.obj


BLOCKS_CODE=evtdly.obj cstblk.obj \
	lusat.obj pload.obj qzcel.obj qzflr.obj\
	qzrnd.obj qztrn.obj scope.obj lsplit.obj csslti.obj\
	dsslti.obj trash.obj zcross.obj \
	expblk.obj logblk.obj sinblk.obj tanblk.obj powblk.obj\
	sqrblk.obj delay.obj selblk.obj forblk.obj  writef.obj invblk.obj hltblk.obj\
	gensin.obj rndblk.obj lookup.obj timblk.obj gensqr.obj mfclck.obj\
	sawtth.obj tcslti.obj tcsltj.obj scopxy.obj evscpe.obj integr.obj readf.obj affich.obj\
	intpol.obj intplt.obj minblk.obj maxblk.obj dlradp.obj  iocopy.obj \
	sum2.obj sum3.obj delayv.obj mux.obj demux.obj samphold.obj dollar.obj mscope.obj \
	intrp2.obj   intrpl.obj fsv.obj memo.obj fscope.obj scoxy.obj \
	diffblk.obj constraint.obj ifthel.obj eselect.obj


BLOCKSC=selector.obj sum.obj prod.obj switchn.obj relay.obj readc.obj writec.obj writeau.obj \
	readau.obj plusblk.obj slider.obj  zcross2.obj mswitch.obj logicalop.obj \
	switch2.obj variable_delay.obj time_delay.obj cscope.obj cmscope.obj \
	satur.obj step_func.obj integral_func.obj absolute_value.obj bounce_ball.obj \
	bouncexy.obj extractor.obj scalar2vector.obj minmax.obj signum.obj product.obj \
	summation.obj multiplex.obj gainblk.obj relationalop.obj modulo_count.obj

OBJSF=intcos.obj coselm.obj sciblk.obj  \
	sctree.obj ftree2.obj ftree3.obj ftree4.obj skipvars.obj scierr.obj scifunc.obj \
	list2vars.obj dtosci.obj itosci.obj scitoi.obj scitod.obj vvtosci.obj \
	scitovv.obj  $(BLOCKS_CODE)

OBJSC=scicos.obj import.obj sciblk2.obj sciblk2i.obj  realtime.obj \
	intrealtime.obj intcscicos.obj blocks.obj cpass2.obj scicosclip.obj \
	sciblk4.obj str2sci.obj intconnection.obj scicos_malloc.obj \
	scicos_free.obj $(BLOCKSC)

BLOCKS=$(BLOCKSF) $(BLOCKSC)

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

#---------------Blocks 

include Make.Blocks.mak

distclean::
	$(RM) Fblocknames Cblocknames blocks.h

#--------------dependencies 
affich.obj: ../stack.h
coselm.obj: ../stack.h
dtosci.obj: ../stack.h
intcos.obj: ../stack.h
itosci.obj: ../stack.h
list2vars.obj: ../stack.h
readf.obj: ../stack.h
sciblk.obj: ../stack.h
scierr.obj: ../stack.h
scifunc.obj: ../stack.h
scitod.obj: ../stack.h
scitoi.obj: ../stack.h
scitovv.obj: ../stack.h
skipvars.obj: ../stack.h
vvtosci.obj: ../stack.h
writef.obj: ../stack.h
writeau.obj: ../stack.h ../machine.h
readau.obj: ../machine.h
grblk.obj: ../machine.h import.h
scicos.obj: ../machine.h import.h blocks.h ../sun/link.h scicos.h scicos_block.h
coselm.obj : ../stack.h
intcos.obj : ../stack.h
cpass2.obj: ../machine.h cc_pass2.h
intconnection.obj: ../machine.h ../stack-c.h
intcscicos.obj: intcscicos.h ../stack-c.h ../machine.h
mswitch.obj: scicos_block.h ../machine.h
logicalop.obj : scicos_block.h ../machine.h
switch2.obj : scicos_block.h ../machine.h
variable_delay.obj : scicos_block.h ../machine.h
time_delay.obj : scicos_block.h ../machine.h
cscope.obj : scicos_block.h ../machine.h
cmscope.obj : scicos_block.h ../machine.h
satur.obj: scicos_block.h ../machine.h
step_func.obj: scicos_block.h ../machine.h
integral_func.obj: scicos_block.h ../machine.h
absolute_value.obj : scicos_block.h ../machine.h
bounce_ball.obj : scicos_block.h ../machine.h
bouncexy.obj : scicos_block.h ../machine.h
extractor.obj : scicos_block.h ../machine.h
scalar2vector.obj : scicos_block.h ../machine.h
minmax.obj : scicos_block.h ../machine.h
signum.obj : scicos_block.h ../machine.h
product.obj : scicos_block.h ../machine.h
summation.obj : scicos_block.h ../machine.h
multiplex.obj : scicos_block.h ../machine.h
gainblk.obj: scicos_block.h ../machine.h
relationalop.obj: scicos_block.h ../machine.h
