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
	readau.o plusblk.o slider.o  zcross2.o

OBJSF=intcos.o coselm.o sciblk.o  \
	sctree.o ftree2.o ftree3.o ftree4.o skipvars.o scierr.o scifunc.o \
	list2vars.o dtosci.o itosci.o scitoi.o scitod.o vvtosci.o \
	scitovv.o  $(BLOCKS_CODE)

OBJSC=scicos.o import.o sciblk2.o sciblk2i.o  realtime.o \
	intrealtime.o duplicate.o blocks.o cpass2.o scicosclip.o \
	sciblk4.o str2sci.o $(BLOCKSC)

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


blocks.o: blocks.c ../machine.h ../calelm/calelm.h scicos.h \
  scicos_block.h
cpass2.o: cpass2.c ../machine.h cc_pass2.h
duplicate.o: duplicate.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
import.o: import.c ../machine.h import.h
intrealtime.o: intrealtime.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
plusblk.o: plusblk.c ../machine.h
prod.o: prod.c ../machine.h
readau.o: readau.c ../machine.h
readc.o: readc.c ../machine.h
realtime.o: realtime.c ../machine.h
relay.o: relay.c ../machine.h
sciblk2.o: sciblk2.c ../machine.h
sciblk2i.o: sciblk2i.c ../machine.h
sciblk4.o: sciblk4.c ../machine.h ../stack-c.h ../graphics/Math.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h scicos_block.h
scicos.o: scicos.c ../machine.h ../sun/link.h scicos.h scicos_block.h \
  import.h blocks.h
scicosclip.o: scicosclip.c ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h ../graphics/PloEch.h
selector.o: selector.c ../machine.h
slider.o: slider.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
str2sci.o: str2sci.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
sum.o: sum.c ../machine.h
switchn.o: switchn.c ../machine.h
writeau.o: writeau.c ../machine.h
writec.o: writec.c ../machine.h
zcross2.o: zcross2.c ../machine.h
