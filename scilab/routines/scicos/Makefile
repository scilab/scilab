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
	summation.o multiplex.o gainblk.o relationalop.o modulo_count.o \
	hystheresis.o ratelimiter.o backlash.o deadband.o ramp.o evaluate_expr.o \
	deriv.o sin_blk.o cos_blk.o tan_blk.o asin_blk.o acos_blk.o atan_blk.o \
        sinh_blk.o cosh_blk.o tanh_blk.o asinh_blk.o acosh_blk.o atanh_blk.o evtvardly.o

OBJSF=
OBJSC= blocks.o blocks_new.o coselm.o cpass2.o evaluate_expr.o ftree2.o \
	ftree3.o ftree4.o import.o intconnection.o intcos.o intcscicos.o \
	intrealtime.o readf.o realtime.o sciblk2.o scicos.o scicosclip.o \
	scicos_free.o scicos_malloc.o scifunc.o 



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

