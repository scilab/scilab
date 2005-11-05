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
        sinh_blk.obj cosh_blk.obj tanh_blk.obj asinh_blk.obj acosh_blk.obj atanh_blk.obj \
	evtvardly.obj edgetrig.obj tcslti4.obj tcsltj4.obj dsslti4.obj \
	csslti4.obj cstblk4.obj samphold4.obj dollar4.obj invblk4.obj delay4.obj

OBJSF=intcos.obj coselm.obj sciblk.obj  \
	sctree.obj ftree2.obj ftree3.obj ftree4.obj skipvars.obj scierr.obj scifunc.obj \
	list2vars.obj scitoi.obj scitod.obj vvtosci.obj \
	scitovv.obj  $(BLOCKS_CODE)

OBJSC=scicos.obj import.obj sciblk2.obj sciblk2i.obj  realtime.obj \
	intrealtime.obj matscicos.obj intcscicos.obj blocks.obj cpass2.obj scicosclip.obj \
	sciblk4.obj intconnection.obj scicos_malloc.obj \
	scicos_free.obj tree.obj $(BLOCKSC)

BLOCKS=$(BLOCKSF) $(BLOCKSC)

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

#---------------Blocks 

include Make.Blocks.mak

distclean::
	$(RM) Fblocknames Cblocknames blocks.h

#--------------dependencies 
# fortran code
affich.obj: affich.f ../stack.h
coselm.obj: coselm.f ../stack.h
intcos.obj: intcos.f  ../stack.h
list2vars.obj: list2vars.f ../stack.h
readf.obj: readf.f ../stack.h
sciblk.obj: sciblk.f  ../stack.h ../callinter.h
scierr.obj: scierr.f  ../stack.h
scifunc.obj: scifunc.f ../stack.h ../callinter.h
scitod.obj: scitod.f ../stack.h
scitoi.obj: scitoi.f ../stack.h
scitovv.obj: scitovv.f ../stack.h
skipvars.obj: skipvars.f  ../stack.h
vvtosci.obj: vvtosci.f  ../stack.h
writef.obj: writef.f ../stack.h

#c code
absolute_value.obj: absolute_value.c scicos_block.h ../machine.h
backlash.obj: backlash.c scicos_block.h
blocks.obj: blocks.c ../machine.h ../calelm/calelm.h scicos.h \
  scicos_block.h
bounce_ball.obj: bounce_ball.c scicos_block.h ../machine.h
bouncexy.obj: bouncexy.c scicos_block.h ../machine.h
cmscope.obj: cmscope.c scicos_block.h ../machine.h
cpass2.obj: cpass2.c ../machine.h cc_pass2.h
cscope.obj: cscope.c scicos_block.h ../machine.h
deadband.obj: deadband.c scicos_block.h
deriv.obj: deriv.c scicos_block.h
extractor.obj: extractor.c scicos_block.h
gainblk.obj: gainblk.c scicos_block.h 
evaluate_expr.obj:  scicos_block.h
hystheresis.obj: hystheresis.c scicos_block.h
import.obj: import.c ../machine.h import.h
intconnection.obj: intconnection.c ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
intcscicos.obj: intcscicos.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h intcscicos.h scicos_block.h
integral_func.obj: integral_func.c scicos_block.h
intrealtime.obj: intrealtime.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
logicalop.obj: logicalop.c scicos_block.h
minmax.obj: minmax.c scicos_block.h
modulo_count.obj: modulo_count.c scicos_block.h
mswitch.obj: mswitch.c scicos_block.h
multiplex.obj: multiplex.c scicos_block.h
plusblk.obj: plusblk.c ../machine.h
prod.obj: prod.c ../machine.h
product.obj: product.c scicos_block.h
ramp.obj: ramp.c scicos_block.h
ratelimiter.obj: ratelimiter.c scicos_block.h
readau.obj: readau.c ../machine.h
readc.obj: readc.c ../machine.h
realtime.obj: realtime.c ../machine.h
relationalop.obj: relationalop.c scicos_block.h
relay.obj: relay.c ../machine.h
satur.obj: satur.c scicos_block.h
scalar2vector.obj: scalar2vector.c scicos_block.h
sciblk2.obj: sciblk2.c ../machine.h
sciblk2i.obj: sciblk2i.c ../machine.h
sciblk4.obj: sciblk4.c ../machine.h ../stack-c.h ../graphics/Math.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h scicos_block.h
scicos.obj: scicos.c ../machine.h ../os_specific/link.h scicos.h scicos_block.h \
  import.h blocks.h
scicos_free.obj: scicos_free.c scicos_block.h
scicos_malloc.obj: scicos_malloc.c scicos_block.h
scicosclip.obj: scicosclip.c ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h ../graphics/PloEch.h
selector.obj: selector.c ../machine.h
signum.obj: signum.c scicos_block.h
slider.obj: slider.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
step_func.obj: step_func.c scicos_block.h
sum.obj: sum.c ../machine.h
summation.obj: summation.c scicos_block.h
switch2.obj: switch2.c scicos_block.h
switchn.obj: switchn.c ../machine.h
time_delay.obj: time_delay.c scicos_block.h ../machine.h
variable_delay.obj: variable_delay.c scicos_block.h ../machine.h
writeau.obj: writeau.c ../machine.h
writec.obj: writec.c ../machine.h
zcross2.obj: zcross2.c ../machine.h
sin_block.obj: sin_block.c scicos_block.h      
cos_block.obj: cos_block.c scicos_block.h      
tan_block.obj: tan_block.c scicos_block.h      
asin_block.obj: asin_block.c scicos_block.h    
acos_block.obj: acos_block.c scicos_block.h    
atan_block.obj: atan_block.c scicos_block.h    
sinh_block.obj: sinh_block.c scicos_block.h    
cosh_block.obj: cosh_block.c scicos_block.h    
tanh_block.obj: tanh_block.c scicos_block.h    
asinh_block.obj: asinh_block.c scicos_block.h  
acosh_block.obj: acosh_block.c scicos_block.h  
atanh_block.obj: atanh_block.c scicos_block.h  
evtvardly.obj: evtvardly.c  scicos_block.h  
tcslti4.obj: tcslti4.c scicos_block.h 
tcsltj4.obj: tcsltj4.c scicos_block.h 
dsslti4.obj: dsslti4.c scicos_block.h  	
csslti4.obj: csslti4.c scicos_block.h
cstblk4.obj: cstblk4.c scicos_block.h 
samphold4.obj: samphold4.c scicos_block.h
dollar4.obj: dollar4.c scicos_block.h
invblk4.obj: invblk4.c scicos_block.h
delay4.obj: delay4.c scicos_block.h
tree.obj: tree.c ../machine.h ../sci_mem_alloc.h

