SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/metanet.a

OBJSC = metanet.o loadg.o saveg.o show.o connex.o dmtree.o paths.o \
	transc.o files.o myhsearch.o

OBJSF = metane.o arbor.o bandred.o bmatch.o busack.o carete.o \
	cent.o cfc.o chcm.o \
	clique.o clique1.o \
	compc.o compfc.o compmat.o deumesh.o diam.o dijkst.o \
	dfs.o dfs1.o dfs2.o eclat.o flomax.o floqua.o ford.o fordfulk.o \
	frang.o \
	frmtrs.o ftrans.o getran.o hamil.o hullcvex.o kilter.o kiltq.o \
	knapsk.o johns.o l2que.o match.o mesh2b.o meshmesh.o minty.o mintyq.o \
	pcchna.o permuto.o prfmatch.o prim.o prim1.o relax.o seed.o \
	tconex.o visitor.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

metanet.o: ../machine.h ../libcomm/libCalCom.h ../libcomm/libCom.h
loadg.o: ../machine.h mysearch.h defs.h
saveg.o: ../machine.h
show.o: ../machine.h ../libcomm/libCalCom.h ../libcomm/libCom.h netcomm.h
connex.o: ../machine.h
dmtree.o: ../machine.h
paths.o: ../machine.h
transc.o: ../machine.h
myhsearch.o: mysearch.h
metane.o : ../stack.h

all:: Makefile.mak Makefile.amk

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.amk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile
