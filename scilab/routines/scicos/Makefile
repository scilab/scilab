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
        sinh_blk.o cosh_blk.o tanh_blk.o asinh_blk.o acosh_blk.o atanh_blk.o \
	evtvardly.o edgetrig.o tcslti4.o tcsltj4.o dsslti4.o \
	csslti4.o cstblk4.o samphold4.o dollar4.o invblk4.o delay4.o

OBJSF=intcos.o coselm.o sciblk.o  \
	sctree.o ftree2.o ftree3.o ftree4.o skipvars.o scierr.o scifunc.o \
	list2vars.o dtosci.o itosci.o scitoi.o scitod.o vvtosci.o \
	scitovv.o  $(BLOCKS_CODE)

OBJSC=scicos.o import.o sciblk2.o sciblk2i.o  realtime.o \
	intrealtime.o matscicos.o intcscicos.o blocks.o cpass2.o scicosclip.o \
	sciblk4.o str2sci.o intconnection.o scicos_malloc.o \
	scicos_free.o tree.o $(BLOCKSC)

BLOCKS=$(BLOCKSF) $(BLOCKSC)

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

#---------------Blocks 

include Make.Blocks

distclean::
	$(RM) Fblocknames Cblocknames blocks.h

#--------------dependencies 
# fortran code
affich.o: affich.f ../stack.h
coselm.o: coselm.f ../stack.h
dtosci.o: dtosci.f  ../stack.h
intcos.o: intcos.f  ../stack.h
itosci.o: itosci.f  ../stack.h
list2vars.o: list2vars.f ../stack.h
readf.o: readf.f ../stack.h
sciblk.o: sciblk.f  ../stack.h ../callinter.h
scierr.o: scierr.f  ../stack.h
scifunc.o: scifunc.f ../stack.h ../callinter.h
scitod.o: scitod.f ../stack.h
scitoi.o: scitoi.f ../stack.h
scitovv.o: scitovv.f ../stack.h
skipvars.o: skipvars.f  ../stack.h
vvtosci.o: vvtosci.f  ../stack.h
writef.o: writef.f ../stack.h

#c code
absolute_value.o: absolute_value.c scicos_block.h ../machine.h
backlash.o: backlash.c scicos_block.h
blocks.o: blocks.c ../machine.h ../calelm/calelm.h scicos.h \
  scicos_block.h
bounce_ball.o: bounce_ball.c scicos_block.h ../machine.h
bouncexy.o: bouncexy.c scicos_block.h ../machine.h
cmscope.o: cmscope.c scicos_block.h ../machine.h
cpass2.o: cpass2.c ../machine.h cc_pass2.h
cscope.o: cscope.c scicos_block.h ../machine.h
deadband.o: deadband.c scicos_block.h
deriv.o: deriv.c scicos_block.h
extractor.o: extractor.c scicos_block.h
gainblk.o: gainblk.c scicos_block.h 
evaluate_expr.o:  scicos_block.h
hystheresis.o: hystheresis.c scicos_block.h
import.o: import.c ../machine.h import.h
intconnection.o: intconnection.c ../stack-c.h ../graphics/Math.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
intcscicos.o: intcscicos.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h intcscicos.h scicos_block.h
integral_func.o: integral_func.c scicos_block.h
intrealtime.o: intrealtime.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
logicalop.o: logicalop.c scicos_block.h
minmax.o: minmax.c scicos_block.h
modulo_count.o: modulo_count.c scicos_block.h
mswitch.o: mswitch.c scicos_block.h
multiplex.o: multiplex.c scicos_block.h
plusblk.o: plusblk.c ../machine.h
prod.o: prod.c ../machine.h
product.o: product.c scicos_block.h
ramp.o: ramp.c scicos_block.h
ratelimiter.o: ratelimiter.c scicos_block.h
readau.o: readau.c ../machine.h
readc.o: readc.c ../machine.h
realtime.o: realtime.c ../machine.h
relationalop.o: relationalop.c scicos_block.h
relay.o: relay.c ../machine.h
satur.o: satur.c scicos_block.h
scalar2vector.o: scalar2vector.c scicos_block.h
sciblk2.o: sciblk2.c ../machine.h
sciblk2i.o: sciblk2i.c ../machine.h
sciblk4.o: sciblk4.c ../machine.h ../stack-c.h ../graphics/Math.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h scicos_block.h
scicos.o: scicos.c ../machine.h ../os_specific/link.h scicos.h scicos_block.h \
  import.h blocks.h
scicos_free.o: scicos_free.c scicos_block.h
scicos_malloc.o: scicos_malloc.c scicos_block.h
scicosclip.o: scicosclip.c ../machine.h ../graphics/Math.h \
  ../graphics/Graphics.h ../graphics/PloEch.h
selector.o: selector.c ../machine.h
signum.o: signum.c scicos_block.h
slider.o: slider.c ../graphics/Math.h ../machine.h ../graphics/Graphics.h
step_func.o: step_func.c scicos_block.h
str2sci.o: str2sci.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
sum.o: sum.c ../machine.h
summation.o: summation.c scicos_block.h
switch2.o: switch2.c scicos_block.h
switchn.o: switchn.c ../machine.h
time_delay.o: time_delay.c scicos_block.h ../machine.h
variable_delay.o: variable_delay.c scicos_block.h ../machine.h
writeau.o: writeau.c ../machine.h
writec.o: writec.c ../machine.h
zcross2.o: zcross2.c ../machine.h
sin_block.o: sin_block.c scicos_block.h      
cos_block.o: cos_block.c scicos_block.h      
tan_block.o: tan_block.c scicos_block.h      
asin_block.o: asin_block.c scicos_block.h    
acos_block.o: acos_block.c scicos_block.h    
atan_block.o: atan_block.c scicos_block.h    
sinh_block.o: sinh_block.c scicos_block.h    
cosh_block.o: cosh_block.c scicos_block.h    
tanh_block.o: tanh_block.c scicos_block.h    
asinh_block.o: asinh_block.c scicos_block.h  
acosh_block.o: acosh_block.c scicos_block.h  
atanh_block.o: atanh_block.c scicos_block.h  
evtvardly.o: evtvardly.c  scicos_block.h  
tcslti4.o: tcslti4.c scicos_block.h 
tcsltj4.o: tcsltj4.c scicos_block.h 
dsslti4.o: dsslti4.c scicos_block.h  	
csslti4.o: csslti4.c scicos_block.h
cstblk4.o: cstblk4.c scicos_block.h 
samphold4.o: samphold4.c scicos_block.h
dollar4.o: dollar4.c scicos_block.h
invblk4.o: invblk4.c scicos_block.h
delay4.o: delay4.c scicos_block.h
tree.o: tree.c ../machine.h ../sci_mem_alloc.h

