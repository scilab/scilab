#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/metanet.lib

OBJSC = loadg.obj saveg.obj connex.obj dmtree.obj paths.obj \
	transc.obj files.obj myhsearch.obj

OBJSF = metane.obj arbor.obj bandred.obj bmatch.obj busack.obj carete.obj \
	cent.obj cfc.obj chcm.obj \
	clique.obj clique1.obj \
	compc.obj compfc.obj compmat.obj deumesh.obj diam.obj dijkst.obj \
	dfs.obj dfs1.obj dfs2.obj eclat.obj flomax.obj floqua.obj ford.obj fordfulk.obj \
	frang.obj \
	frmtrs.obj ftrans.obj getran.obj hamil.obj hullcvex.obj kilter.obj kiltq.obj \
	knapsk.obj johns.obj l2que.obj match.obj mesh2b.obj meshmesh.obj minty.obj mintyq.obj \
	pcchna.obj permuto.obj prfmatch.obj prim.obj prim1.obj relax.obj seed.obj \
	tconex.obj visitor.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

metane.obj: ../stack.h

connex.obj: connex.c ../machine.h
dmtree.obj: dmtree.c ../machine.h
files.obj: files.c
loadg.obj: loadg.c mysearch.h ../machine.h defs.h
myhsearch.obj: myhsearch.c mysearch.h
paths.obj: paths.c ../machine.h
saveg.obj: saveg.c ../machine.h
transc.obj: transc.c ../machine.h

