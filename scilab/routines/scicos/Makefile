SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/scicos.a

BLOCKSF=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o\
	qzrnd.o qztrn.o scope.o lsplit.o csslti.o\
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o\
	sqrblk.o delay.o selblk.o forblk.o ifthel.o writef.o invblk.o hltblk.o\
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o\
	sawtth.o tcslti.o tcsltj.o scopxy.o evscpe.o integr.o readf.o affich.o\
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o mscope.o \
	eselect.o intrp2.o   intrpl.o fsv.o memo.o fscope.o scoxy.o diffblk.o constraint.o \
	absblk.o andlog.o bidon.o gain.o cdummy.o dband.o cosblk.o 


BLOCKS_CODE=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o\
	qzrnd.o qztrn.o scope.o lsplit.o csslti.o\
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o\
	sqrblk.o delay.o selblk.o forblk.o ifthel.o writef.o invblk.o hltblk.o\
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o\
	sawtth.o tcslti.o tcsltj.o scopxy.o evscpe.o integr.o readf.o affich.o\
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o mscope.o \
	eselect.o intrp2.o   intrpl.o fsv.o memo.o fscope.o scoxy.o \
	diffblk.o constraint.o 


BLOCKSC=selector.o sum.o prod.o switchn.o relay.o readc.o writec.o writeau.o \
	readau.o plusblk.o slider.o 

OBJSF=intcos.o coselm.o sciblk.o  addevt.o doit.o odoit.o zdoit.o scicos.o \
	cosini.o cossim.o cosend.o \
	sctree.o ftree2.o ftree3.o ftree4.o skipvars.o scierr.o scifunc.o \
	list2vars.o dtosci.o itosci.o scitoi.o scitod.o vvtosci.o \
	scitovv.o cosiord.o $(BLOCKS_CODE)

OBJSC=callf.o import.o sciblk2.o sciblk2i.o simblk.o grblk.o realtime.o \
	intrealtime.o duplicate.o blocks.o cpass2.o $(BLOCKSC)

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

callf.o: blocks.h ../machine.h ../sun/link.h scicos.h
scifunc.o : ../stack.h ../callinter.h
selector.o: ../machine.h
import.o: ../machine.h import.h
prod.o: ../machine.h
sciblk2.o: ../machine.h
sum.o: ../machine.h
switchn.0: ../machine.h

sciblk.o : ../stack.h ../callinter.h
dtosci.o: ../stack.h
itosci.o: ../stack.h
list2vars.o: ../stack.h
readf.o: ../stack.h
readf.o: ../stack.h
sciblk.o: ../stack.h ../callinter.h
scierr.o: ../stack.h
scitod.o: ../stack.h
scitoi.o: ../stack.h
scitovv.o: ../stack.h
skipvars.o: ../stack.h
vvtosci.o: ../stack.h
writef.o: ../stack.h
writeau.o: ../stack.h ../machine.h
readau.o: ../machine.h
grblk.o: ../machine.h import.h
simblk.o: ../machine.h import.h
coselm.o : ../stack.h
intcos.o : ../stack.h
cpass2.o: ../machine.h

