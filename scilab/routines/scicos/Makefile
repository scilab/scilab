SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/scicos.a

BLOCKSF=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o\
	qzrnd.o qztrn.o scope.o lsplit.o csslti.o\
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o\
	sqrblk.o delay.o selblk.o forblk.o writef.o invblk.o hltblk.o\
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o\
	sawtth.o tcslti.o tcsltj.o scopxy.o evscpe.o integr.o readf.o affich.o\
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o mscope.o \
	intrp2.o   intrpl.o fsv.o memo.o fscope.o scoxy.o diffblk.o constraint.o \
	absblk.o andlog.o bidon.o gain.o cdummy.o dband.o cosblk.o ifthel.o eselect.o


BLOCKS_CODE=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o\
	qzrnd.o qztrn.o scope.o lsplit.o csslti.o\
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o\
	sqrblk.o delay.o selblk.o forblk.o  writef.o invblk.o hltblk.o\
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o\
	sawtth.o tcslti.o tcsltj.o scopxy.o evscpe.o integr.o readf.o affich.o\
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o mscope.o \
	intrp2.o   intrpl.o fsv.o memo.o fscope.o scoxy.o \
	diffblk.o constraint.o ifthel.o eselect.o


BLOCKSC=selector.o sum.o prod.o switchn.o relay.o readc.o writec.o writeau.o \
	readau.o plusblk.o slider.o  zcross2.o mswitch.o logicalop.o \
	switch2.o variable_delay.o time_delay.o cscope.o cmscope.o \
	satur.o step_func.o integral_func.o absolute_value.o bounce_ball.o \
	bouncexy.o extractor.o scalar2vector.o minmax.o signum.o product.o \
	summation.o multiplex.o gainblk.o relationalop.o modulo_count.o

OBJSF=intcos.o coselm.o sciblk.o  \
	sctree.o ftree2.o ftree3.o ftree4.o skipvars.o scierr.o scifunc.o \
	list2vars.o dtosci.o itosci.o scitoi.o scitod.o vvtosci.o \
	scitovv.o  $(BLOCKS_CODE)

OBJSC=scicos.o import.o sciblk2.o sciblk2i.o  realtime.o \
	intrealtime.o intcscicos.o blocks.o cpass2.o scicosclip.o \
	sciblk4.o str2sci.o intconnection.o scicos_malloc.o \
	scicos_free.o $(BLOCKSC)

BLOCKS=$(BLOCKSF) $(BLOCKSC)

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

#---------------Blocks 

include Make.Blocks

distclean::
	$(RM) Fblocknames Cblocknames blocks.h

#--------------dependencies 
affich.o: ../stack.h
coselm.o: ../stack.h
dtosci.o: ../stack.h
intcos.o: ../stack.h
itosci.o: ../stack.h
list2vars.o: ../stack.h
readf.o: ../stack.h
sciblk.o: ../stack.h
scierr.o: ../stack.h
scifunc.o: ../stack.h
scitod.o: ../stack.h
scitoi.o: ../stack.h
scitovv.o: ../stack.h
skipvars.o: ../stack.h
vvtosci.o: ../stack.h
writef.o: ../stack.h
writeau.o: ../stack.h ../machine.h
readau.o: ../machine.h
grblk.o: ../machine.h import.h
scicos.o: ../machine.h import.h blocks.h ../sun/link.h scicos.h scicos_block.h
coselm.o : ../stack.h
intcos.o : ../stack.h
cpass2.o: ../machine.h cc_pass2.h
intconnection.o: ../machine.h ../stack-c.h
intcscicos.o: intcscicos.h ../stack-c.h ../machine.h
mswitch.o: scicos_block.h ../machine.h
logicalop.o : scicos_block.h ../machine.h
switch2.o : scicos_block.h ../machine.h
variable_delay.o : scicos_block.h ../machine.h
time_delay.o : scicos_block.h ../machine.h
cscope.o : scicos_block.h ../machine.h
cmscope.o : scicos_block.h ../machine.h
satur.o: scicos_block.h ../machine.h
step_func.o: scicos_block.h ../machine.h
integral_func.o: scicos_block.h ../machine.h
absolute_value.o : scicos_block.h ../machine.h
bounce_ball.o : scicos_block.h ../machine.h
bouncexy.o : scicos_block.h ../machine.h
extractor.o : scicos_block.h ../machine.h
scalar2vector.o : scicos_block.h ../machine.h
minmax.o : scicos_block.h ../machine.h
signum.o : scicos_block.h ../machine.h
product.o : scicos_block.h ../machine.h
summation.o : scicos_block.h ../machine.h
multiplex.o : scicos_block.h ../machine.h
gainblk.o: scicos_block.h ../machine.h
relationalop.o: scicos_block.h ../machine.h
modulo_count.o: scicos_block.h ../machine.h
