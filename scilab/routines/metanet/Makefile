SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/metanet.a

OBJSC = loadg.o saveg.o connex.o dmtree.o paths.o \
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


metane.o: ../stack.h

connex.o: connex.c ../machine.h
dmtree.o: dmtree.c ../machine.h
files.o: files.c
loadg.o: loadg.c mysearch.h ../machine.h defs.h
myhsearch.o: myhsearch.c mysearch.h
paths.o: paths.c ../machine.h
saveg.o: saveg.c ../machine.h
transc.o: transc.c ../machine.h

