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
	summation.obj multiplex.obj gainblk.obj relationalop.obj modulo_count.obj \
	hystheresis.obj ratelimiter.obj backlash.obj deadband.obj ramp.obj evaluate_expr.obj \
	deriv.obj sin_blk.obj cos_blk.obj tan_blk.obj asin_blk.obj acos_blk.obj atan_blk.obj \
        sinh_blk.obj cosh_blk.obj tanh_blk.obj asinh_blk.obj acosh_blk.obj atanh_blk.obj evtvardly.obj

OBJSF=
OBJSC= blocks.obj blocks_new.obj coselm.obj cpass2.obj evaluate_expr.obj ftree2.obj \
	ftree3.obj ftree4.obj import.obj intconnection.obj intcos.obj intcscicos.obj \
	intrealtime.obj readf.obj realtime.obj sciblk2.obj scicos.obj scicosclip.obj \
	scicos_free.obj scicos_malloc.obj scifunc.obj 



BLOCKS=$(BLOCKSF) $(BLOCKSC)

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

Makefile.libmk	: Makefile
	$(SCIDIR)/util/Mak2ABSMak Makefile

#---------------Blocks 

include Make.Blocks.mak

distclean::
	$(RM) Fblocknames Cblocknames blocks.h

#--------------dependencies 

