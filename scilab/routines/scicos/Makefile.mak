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
	readau.obj plusblk.obj slider.obj  zcross2.obj

OBJSF=intcos.obj coselm.obj sciblk.obj  \
	sctree.obj ftree2.obj ftree3.obj ftree4.obj skipvars.obj scierr.obj scifunc.obj \
	list2vars.obj dtosci.obj itosci.obj scitoi.obj scitod.obj vvtosci.obj \
	scitovv.obj  $(BLOCKS_CODE)

OBJSC=scicos.obj import.obj sciblk2.obj sciblk2i.obj  realtime.obj \
	intrealtime.obj duplicate.obj blocks.obj cpass2.obj scicosclip.obj \
	sciblk4.obj str2sci.obj $(BLOCKSC)

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


blocks.obj: blocks.c ../machine.h ../calelm/calelm.h scicos.h \
  scicos_block.h
cpass2.obj: cpass2.c ../machine.h cc_pass2.h
duplicate.obj: duplicate.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
import.obj: import.c ../machine.h import.h
intrealtime.obj: intrealtime.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
plusblk.obj: plusblk.c ../machine.h
prod.obj: prod.c ../machine.h
readau.obj: readau.c ../machine.h
readc.obj: readc.c ../machine.h
realtime.obj: realtime.c ../machine.h
relay.obj: relay.c ../machine.h
sciblk2.obj: sciblk2.c ../machine.h
sciblk2i.obj: sciblk2i.c ../machine.h
sciblk4.obj: sciblk4.c ../machine.h ../stack-c.h ../graphics/Math.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h scicos_block.h
scicos.obj: scicos.c ../machine.h ../sun/link.h scicos.h scicos_block.h \
  import.h blocks.h
scicosclip.obj: scicosclip.c ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h ../graphics/PloEch.h
selector.obj: selector.c ../machine.h
slider.obj: slider.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
str2sci.obj: str2sci.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
sum.obj: sum.c ../machine.h
switchn.obj: switchn.c ../machine.h
writeau.obj: writeau.c ../machine.h
writec.obj: writec.c ../machine.h
zcross2.obj: zcross2.c ../machine.h
