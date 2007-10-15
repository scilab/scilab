SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/scicos.a

BLOCKSF=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o \
	qzrnd.o qztrn.o lsplit.o csslti.o \
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o \
	sqrblk.o delay.o selblk.o forblk.o writef.o invblk.o hltblk.o \
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o \
	sawtth.o tcslti.o tcsltj.o integr.o readf.o affich2.o affich.o \
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o \
	intrp2.o intrpl.o fsv.o memo.o diffblk.o constraint.o \
	absblk.o andlog.o bidon.o gain.o cdummy.o dband.o cosblk.o ifthel.o \
	eselect.o


BLOCKS_CODE=evtdly.o cstblk.o \
	lusat.o pload.o qzcel.o qzflr.o \
	qzrnd.o qztrn.o lsplit.o csslti.o \
	dsslti.o trash.o zcross.o \
	expblk.o logblk.o sinblk.o tanblk.o powblk.o \
	sqrblk.o delay.o selblk.o forblk.o  writef.o invblk.o hltblk.o \
	gensin.o rndblk.o lookup.o timblk.o gensqr.o mfclck.o \
	sawtth.o tcslti.o tcsltj.o integr.o readf.o affich2.o affich.o \
	intpol.o intplt.o minblk.o maxblk.o dlradp.o  iocopy.o \
	sum2.o sum3.o delayv.o mux.o demux.o samphold.o dollar.o \
	intrp2.o intrpl.o fsv.o memo.o \
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
	csslti4.o cstblk4.o samphold4.o dollar4.o invblk4.o delay4.o \
	cevscpe.o cfscope.o cscopxy.o canimxy.o canimxy3d.o cscopxy3d.o \
	matmul_m.o mattran_m.o cmatview.o cmat3d.o \
	extdiag.o exttril.o mat_bksl.o mat_diag.o mat_lu.o mat_svd.o \
	matz_absc.o matz_conj.o matz_expm.o matz_reim.o matz_svd.o root_coef.o \
	extdiagz.o exttrilz.o mat_cath.o mat_div.o mat_pinv.o mat_vps.o matz_bksl.o \
	matz_det.o matz_inv.o matz_reimc.o matz_vps.o rootz_coef.o extract.o exttriu.o \
	mat_catv.o mat_expm.o mat_reshape.o mat_vpv.o matz_cath.o matz_diag.o matz_lu.o \
	matz_reshape.o matz_vpv.o submat.o extractz.o exttriuz.o mat_det.o mat_inv.o \
	mat_sing.o matz_abs.o matz_catv.o matz_div.o matz_pinv.o matz_sing.o ricc_m.o \
	submatz.o switch2_m.o dollar4_m.o cstblk4_m.o integralz_func.o \
        matzmul_m.o matztran_m.o mat_sum.o mat_sumc.o mat_suml.o cumsum_c.o cumsum_m.o \
	cumsum_r.o matz_sum.o matz_sumc.o matz_suml.o cumsumz_c.o cumsumz_m.o \
	cumsumz_r.o selector_m.o summation_z.o \
	convert.o logicalop_i32.o logicalop_ui32.o logicalop_i16.o logicalop_ui16.o \
	logicalop_i8.o logicalop_ui8.o logicalop_m.o samphold4_m.o matmul_i32s.o \
	matmul_i32n.o matmul_i32e.o matmul_i16s.o matmul_i16n.o matmul_i16e.o \
	matmul_i8s.o matmul_i8n.o matmul_i8e.o matmul_ui32s.o matmul_ui32n.o \
	matmul_ui32e.o matmul_ui16s.o matmul_ui16n.o matmul_ui16e.o matmul_ui8s.o \
	matmul_ui8n.o matmul_ui8e.o summation_i32s.o summation_i32n.o summation_i32e.o \
	summation_i16s.o summation_i16n.o summation_i16e.o summation_i8s.o \
	summation_i8n.o summation_i8e.o summation_ui32s.o summation_ui32n.o \
	summation_ui32e.o summation_ui16s.o summation_ui16n.o summation_ui16e.o \
	summation_ui8s.o summation_ui8n.o summation_ui8e.o gainblk_i32s.o \
	gainblk_i32n.o gainblk_i32e.o gainblk_i16s.o gainblk_i16n.o gainblk_i16e.o \
	gainblk_i8s.o gainblk_i8n.o gainblk_i8e.o gainblk_ui32s.o gainblk_ui32n.o \
	gainblk_ui32e.o gainblk_ui16s.o gainblk_ui16n.o gainblk_ui16e.o gainblk_ui8s.o \
	gainblk_ui8n.o gainblk_ui8e.o delay4_i32.o delay4_i16.o delay4_i8.o delay4_ui32.o \
	delay4_ui16.o delay4_ui8.o mat_sqrt.o \
	matz_sqrt.o relational_op_i32.o relational_op_ui32.o relational_op_i16.o \
	relational_op_ui16.o relational_op_i8.o relational_op_ui8.o evtdly4.o\
	matmul2_m.o matzmul2_m.o expblk_m.o logic.o bit_clear_32.o bit_clear_16.o bit_clear_8.o\
	bit_set_32.o bit_set_16.o bit_set_8.o extract_bit_32_UH0.o extract_bit_16_UH0.o \
	extract_bit_8_UH0.o extract_bit_32_UH1.o extract_bit_16_UH1.o extract_bit_8_UH1.o \
	extract_bit_32_LH.o extract_bit_16_LH.o extract_bit_8_LH.o extract_bit_32_MSB0.o \
        extract_bit_16_MSB0.o extract_bit_8_MSB0.o extract_bit_32_MSB1.o extract_bit_16_MSB1.o \
        extract_bit_8_MSB1.o extract_bit_32_LSB.o extract_bit_16_LSB.o extract_bit_8_LSB.o \
        extract_bit_32_RB0.o extract_bit_16_RB0.o extract_bit_8_RB0.o extract_bit_32_RB1.o \
        extract_bit_16_RB1.o extract_bit_8_RB1.o shift_8_LA.o shift_16_LA.o shift_32_LA.o \
	shift_8_LC.o shift_16_LC.o shift_32_LC.o shift_8_RA.o shift_16_RA.o shift_32_RA.o \
	shift_8_RC.o shift_16_RC.o shift_32_RC.o shift_u8_RA.o shift_u16_RA.o shift_u32_RA.o\
	extract_bit_u32_UH1.o extract_bit_u16_UH1.o extract_bit_u8_UH1.o extract_bit_u32_MSB1.o\
	extract_bit_u16_MSB1.o extract_bit_u8_MSB1.o extract_bit_u32_RB1.o extract_bit_u16_RB1.o \
        extract_bit_u8_RB1.o rndblk_m.o relational_op.o curve_c.o counter.o m_frequ.o \
        tows_c.o rndblkz_m.o fromws_c.o mathermit_m.o scicosexit.o automat.o

OBJSF=intcos.o coselm.o sciblk.o  \
	sctree.o ftree2.o ftree3.o ftree4.o skipvars.o scierr.o scifunc.o \
	list2vars.o dtosci.o itosci.o scitoi.o scitod.o vvtosci.o \
	scitovv.o  $(BLOCKS_CODE)

SUNDIALS=sundials/cvode.o sundials/cvode_io.o sundials/cvode_dense.o \
	sundials/nvector_serial.o sundials/ida.o sundials/ida_dense.o \
	sundials/ida_io.o sundials/ida_ic.o \
	sundials/sundials_nvector.o sundials/sundials_math.o \
	sundials/sundials_dense.o sundials/sundials_smalldense.o

OBJSC=scicos.o intcscicos.o import.o realtime.o \
	intrealtime.o matscicos.o blocks.o scicosclip.o \
	sciblk2.o sciblk2i.o sciblk4.o str2sci.o scicos_malloc.o \
	scicos_free.o tree.o \
	scoGetProperty.o scoSetProperty.o scoMisc.o \
	scoMemoryScope.o scoWindowScope.o $(BLOCKSC) $(SUNDIALS)

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
affich.o: affich2.f ../stack.h
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
cscope.o: cscope.c scicos_block.h ../machine.h
deadband.o: deadband.c scicos_block.h
deriv.o: deriv.c scicos_block.h
extractor.o: extractor.c scicos_block.h
gainblk.o: gainblk.c scicos_block.h 
evaluate_expr.o:  scicos_block.h
hystheresis.o: hystheresis.c scicos_block.h
import.o: import.c ../machine.h import.h
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
readf4.o: readf4.c ../machine.h scicos_block.h
tree.o: tree.c ../machine.h ../sci_mem_alloc.h
matmul_m.o: matmul_m.c scicos_block4.h
mattran_m.o: mattran_m.c scicos_block4.h
extdiag.o:extdiag.c  scicos_block4.h ../machine.h
exttril.o: exttril.c scicos_block4.h ../machine.h
mat_bksl.o: mat_bksl.c scicos_block4.h ../machine.h
mat_diag.o:mat_diag.c scicos_block4.h ../machine.h
mat_lu.o: mat_lu.c scicos_block4.h ../machine.h
mat_svd.o:mat_svd.c scicos_block4.h ../machine.h
matz_absc.o: matz_absc.c scicos_block4.h ../machine.h
matz_conj.o: matz_conj.c scicos_block4.h ../machine.h
matz_expm.o: matz_expm.c scicos_block4.h ../machine.h
matz_reim.o: matz_reim.c scicos_block4.h ../machine.h
matz_svd.o: matz_svd.c scicos_block4.h ../machine.h
root_coef.o:root_coef.c scicos_block4.h 
extdiagz.o: extdiagz.c scicos_block4.h ../machine.h
exttrilz.o:exttrilz.c scicos_block4.h ../machine.h
mat_cath.o: mat_cath.c scicos_block4.h ../machine.h
mat_div.o: mat_div.c scicos_block4.h ../machine.h
mat_pinv.o: mat_pinv.c scicos_block4.h ../machine.h
mat_vps.o: mat_vps.c scicos_block4.h ../machine.h
matz_bksl.o:matz_bksl.c scicos_block4.h ../machine.h
matz_det.o: matz_det.c scicos_block4.h ../machine.h
matz_inv.o: matz_inv.c scicos_block4.h ../machine.h
matz_reimc.o: matz_reimc.c scicos_block4.h ../machine.h
matz_vps.o: matz_vps.c scicos_block4.h ../machine.h
rootz_coef.o: rootz_coef.c scicos_block4.h
extract.o: extract.c scicos_block4.h ../machine.h
exttriu.o:exttriu.c scicos_block4.h ../machine.h
mat_catv.o: mat_catv.c scicos_block4.h ../machine.h
mat_expm.o: mat_expm.c scicos_block4.h ../machine.h
mat_reshape.o: mat_reshape.c scicos_block4.h ../machine.h
mat_vpv.o: mat_vpv.c scicos_block4.h ../machine.h
matz_cath.o: matz_cath.c scicos_block4.h ../machine.h
matz_diag.o: matz_diag.c scicos_block4.h ../machine.h
matz_lu.o:matz_lu.c scicos_block4.h ../machine.h
matz_reshape.o: matz_reshape.c scicos_block4.h ../machine.h
matz_vpv.o: matz_vpv.c scicos_block4.h ../machine.h
submat.o: submat.c scicos_block4.h ../machine.h
extractz.o: extractz.c scicos_block4.h ../machine.h
exttriuz.o: exttriuz.c scicos_block4.h ../machine.h
mat_det.o: mat_det.c scicos_block4.h ../machine.h
mat_inv.o:mat_inv.c scicos_block4.h ../machine.h
mat_sing.o: mat_sing.c scicos_block4.h ../machine.h
matz_abs.o: matz_abs.c scicos_block4.h ../machine.h
matz_catv.o: matz_catv.c scicos_block4.h ../machine.h
matz_div.o: matz_div.c scicos_block4.h ../machine.h
matz_pinv.o: matz_pinv.c scicos_block4.h ../machine.h
matz_sing.o: matz_sing.c scicos_block4.h ../machine.h
ricc_m.o:ricc_m.c scicos_block4.h ../machine.h
submatz.o: submatz.c scicos_block4.h ../machine.h
switch2_m.o: switch2_m.c scicos_block4.h 
dollar4_m.o: dollar4_m.c scicos_block4.h
cstblk4_m.o: cstblk4_m.c scicos_block4.h
integralz_func.o: integralz_func.c scicos_block4.h
matzmul_m.o: matzmul_m.c scicos_block4.h ../machine.h
matztran_m.o: matztran_m.c scicos_block4.h ../machine.h
mat_sum.o: mat_sum.c scicos_block4.h ../machine.h
mat_sumc.o: mat_sumc.c scicos_block4.h ../machine.h
mat_suml.o: mat_suml.c scicos_block4.h ../machine.h
cumsum_c.o: cumsum_c.c scicos_block4.h ../machine.h
cumsum_m.o: cumsum_m.c scicos_block4.h ../machine.h
cumsum_r.o: cumsum_r.c scicos_block4.h ../machine.h
matz_sum.o: matz_sum.c scicos_block4.h ../machine.h
matz_sumc.o: matz_sumc.c scicos_block4.h ../machine.h
matz_suml.o: matz_suml.c scicos_block4.h ../machine.h
cumsumz_c.o: cumsumz_c.c scicos_block4.h ../machine.h
cumsumz_m.o: cumsumz_m.c scicos_block4.h ../machine.h
cumsumz_r.o: cumsumz_r.c scicos_block4.h ../machine.h
selector_m.o: selector_m.c scicos_block4.h ../machine.h
summation_z.o: summation_z.c scicos_block4.h
convert.o: convert.c scicos_block4.h ../machine.h 
logicalop_i32.o: logicalop_i32.c scicos_block4.h
logicalop_ui32.o: logicalop_ui32.c scicos_block4.h
logicalop_i16.o: logicalop_i16.c scicos_block4.h
logicalop_ui16.o: logicalop_ui16.c scicos_block4.h
logicalop_i8.o: logicalop_i8.c scicos_block4.h
logicalop_ui8.o: logicalop_ui8.c scicos_block4.h
logicalop_m.o: logicalop_m.c scicos_block4.h
samphold4_m.o: samphold4_m.c scicos_block4.h
matmul_i32s.o: matmul_i32s.c scicos_block4.h ../machine.h
matmul_i32n.o: matmul_i32n.c scicos_block4.h ../machine.h
matmul_i32e.o: matmul_i32e.c scicos_block4.h ../machine.h
matmul_i16s.o: matmul_i16s.c scicos_block4.h ../machine.h
matmul_i16n.o: matmul_i16n.c scicos_block4.h ../machine.h
matmul_i16e.o: matmul_i16e.c scicos_block4.h ../machine.h
matmul_i8s.o: matmul_i8s.c scicos_block4.h ../machine.h
matmul_i8n.o: matmul_i8n.c scicos_block4.h ../machine.h
matmul_i8e.o: matmul_i8e.c scicos_block4.h ../machine.h
matmul_ui32s.o: matmul_ui32s.c scicos_block4.h ../machine.h 
matmul_ui32n.o: matmul_ui32n.c scicos_block4.h ../machine.h 
matmul_ui32e.o: matmul_ui32e.c scicos_block4.h ../machine.h
matmul_ui16s.o: matmul_ui16s.c scicos_block4.h ../machine.h
matmul_ui16n.o: matmul_ui16n.c scicos_block4.h ../machine.h
matmul_ui16e.o: matmul_ui16e.c scicos_block4.h ../machine.h
matmul_ui8s.o: matmul_ui8s.c scicos_block4.h ../machine.h
matmul_ui8n.o: matmul_ui8n.c scicos_block4.h ../machine.h
matmul_ui8e.o: matmul_ui8e.c scicos_block4.h ../machine.h
summation_i32s.o: summation_i32s.c scicos_block4.h 
summation_i32n.o: summation_i32n.c scicos_block4.h 
summation_i32e.o: summation_i32e.c scicos_block4.h
summation_i16s.o: summation_i16s.c scicos_block4.h 
summation_i16n.o: summation_i16n.c scicos_block4.h 
summation_i16e.o: summation_i16e.c scicos_block4.h 
summation_i8s.o: summation_i8s.c scicos_block4.h 
summation_i8n.o: summation_i8n.c scicos_block4.h 
summation_i8e.o: summation_i8e.c scicos_block4.h 
summation_ui32s.o: summation_ui32s.c scicos_block4.h 
summation_ui32n.o: summation_ui32n.c scicos_block4.h 
summation_ui32e.o: summation_ui32e.c scicos_block4.h 
summation_ui16s.o: summation_ui16s.c scicos_block4.h
summation_ui16n.o: summation_ui16n.c scicos_block4.h
summation_ui16e.o: summation_ui16e.c scicos_block4.h
summation_ui8s.o: summation_ui8s.c scicos_block4.h
summation_ui8n.o: summation_ui8n.c scicos_block4.h
summation_ui8e.o: summation_ui8e.c scicos_block4.h
gainblk_i32s.o: gainblk_i32s.c scicos_block4.h ../machine.h
gainblk_i32n.o: gainblk_i32n.c scicos_block4.h ../machine.h
gainblk_i32e.o: gainblk_i32e.c scicos_block4.h ../machine.h
gainblk_i16s.o: gainblk_i16s.c scicos_block4.h ../machine.h
gainblk_i16n.o: gainblk_i16n.c scicos_block4.h ../machine.h
gainblk_i16e.o: gainblk_i16e.c scicos_block4.h ../machine.h
gainblk_i8s.o: gainblk_i8s.c scicos_block4.h ../machine.h
gainblk_i8n.o: gainblk_i8n.c scicos_block4.h ../machine.h
gainblk_i8e.o: gainblk_i8e.c scicos_block4.h ../machine.h
gainblk_ui32s.o: gainblk_ui32s.c scicos_block4.h ../machine.h
gainblk_ui32n.o: gainblk_ui32n.c scicos_block4.h ../machine.h
gainblk_ui32e.o: gainblk_ui32e.c scicos_block4.h ../machine.h
gainblk_ui16s.o: gainblk_ui16s.c scicos_block4.h ../machine.h
gainblk_ui16n.o: gainblk_ui16n.c scicos_block4.h ../machine.h
gainblk_ui16e.o: gainblk_ui16e.c scicos_block4.h ../machine.h
gainblk_ui8s.o: gainblk_ui8s.c scicos_block4.h ../machine.h
gainblk_ui8n.o: gainblk_ui8n.c scicos_block4.h ../machine.h
gainblk_ui8e.o: gainblk_ui8e.c scicos_block4.h ../machine.h
delay4_i32.o: delay4_i32.c scicos_block4.h
delay4_i16.o: delay4_i16.c scicos_block4.h
delay4_i8.o: delay4_i8.c scicos_block4.h
delay4_ui32.o: delay4_ui32.c scicos_block4.h
delay4_ui16.o: delay4_ui16.c scicos_block4.h
delay4_ui8.o: delay4_ui8.c scicos_block4.h
mat_sqrt.o: mat_sqrt.c scicos_block4.h ../machine.h
matz_sqrt.o: matz_sqrt.c scicos_block4.h ../machine.h 
relational_op_i32.o: relational_op_i32.c scicos_block4.h
relational_op_ui32.o: relational_op_ui32.c scicos_block4.h
relational_op_i16.o: relational_op_i16.c scicos_block4.h
relational_op_ui16.o: relational_op_ui16.c scicos_block4.h
relational_op_i8.o: relational_op_i8.c scicos_block4.h
relational_op_ui8.o: relational_op_ui8.c scicos_block4.h
evtdly4.o: evtdly4.c scicos_block4.h
matmul2_m.o: matmul2_m.c scicos_block4.h ../machine.h
matzmul2_m.o: matzmul2_m.c scicos_block4.h ../machine.h
expblk_m.o: expblk_m.c scicos_block4.h ../machine.h
logic.o: logic.c scicos_block4.h
bit_clear_32.o: bit_clear_32.c scicos_block4.h
bit_clear_16.o: bit_clear_16.c scicos_block4.h
bit_clear_8.o: bit_clear_8.c scicos_block4.h
bit_set_32.o: bit_set_32.c scicos_block4.h
bit_set_16.o: bit_set_16.c scicos_block4.h
bit_set_8.o: bit_set_8.c scicos_block4.h
extract_bit_32_UH0.o: extract_bit_32_UH0.c scicos_block4.h
extract_bit_16_UH0.o: extract_bit_16_UH0.c scicos_block4.h
extract_bit_8_UH0.o: extract_bit_8_UH0.c scicos_block4.h
extract_bit_32_UH1.o: extract_bit_32_UH1.c scicos_block4.h
extract_bit_16_UH1.o: extract_bit_16_UH1.c scicos_block4.h
extract_bit_8_UH1.o: extract_bit_8_UH1.c scicos_block4.h
extract_bit_32_LH.o: extract_bit_32_LH.c scicos_block4.h
extract_bit_16_LH.o: extract_bit_16_LH.c scicos_block4.h
extract_bit_8_LH.o: extract_bit_8_LH.c scicos_block4.h
extract_bit_32_MSB0.o: extract_bit_32_MSB0.c scicos_block4.h
extract_bit_16_MSB0.o: extract_bit_16_MSB0.c scicos_block4.h
extract_bit_8_MSB0.o: extract_bit_8_MSB0.c scicos_block4.h
extract_bit_32_MSB1.o: extract_bit_32_MSB1.c scicos_block4.h
extract_bit_16_MSB1.o: extract_bit_16_MSB1.c scicos_block4.h
extract_bit_8_MSB1.o: extract_bit_8_MSB1.c scicos_block4.h
extract_bit_32_LSB.o: extract_bit_32_LSB.c scicos_block4.h
extract_bit_16_LSB.o: extract_bit_16_LSB.c scicos_block4.h
extract_bit_8_LSB.o:extract_bit_8_LSB.c scicos_block4.h
extract_bit_32_RB0.o: extract_bit_32_RB0.c scicos_block4.h
extract_bit_16_RB0.o: extract_bit_16_RB0.c scicos_block4.h
extract_bit_8_RB0.o: extract_bit_8_RB0.c scicos_block4.h
extract_bit_32_RB1.o: extract_bit_32_RB1.c scicos_block4.h
extract_bit_16_RB1.o: extract_bit_16_RB1.c scicos_block4.h
extract_bit_8_RB1.o: extract_bit_8_RB1.c scicos_block4.h
shift_8_LA.o: shift_8_LA.c scicos_block4.h ../machine.h
shift_16_LA.o: shift_16_LA.c scicos_block4.h ../machine.h
shift_32_LA.o: shift_32_LA.c scicos_block4.h ../machine.h
shift_8_LC.o: shift_8_LC.c scicos_block4.h ../machine.h
shift_16_LC.o: shift_16_LC.c scicos_block4.h ../machine.h
shift_32_LC.o: shift_32_LC.c scicos_block4.h ../machine.h
shift_8_RA.o: shift_8_RA.c scicos_block4.h ../machine.h
shift_16_RA.o: shift_16_RA.c scicos_block4.h ../machine.h
shift_32_RA.o: shift_32_RA.c scicos_block4.h ../machine.h
shift_8_RC.o: shift_8_RC.c scicos_block4.h ../machine.h
shift_16_RC.o: shift_16_RC.c scicos_block4.h ../machine.h
shift_32_RC.o: shift_32_RC.c scicos_block4.h ../machine.h
shift_u8_RA.o: shift_u8_RA.c scicos_block4.h ../machine.h
shift_u16_RA.o: shift_u16_RA.c scicos_block4.h ../machine.h
shift_u32_RA.o: shift_u32_RA.c scicos_block4.h ../machine.h
extract_bit_u32_UH1.o: extract_bit_u32_UH1.c scicos_block4.h
extract_bit_u16_UH1.o: extract_bit_u16_UH1.c scicos_block4.h
extract_bit_u8_UH1.o: extract_bit_u8_UH1.c scicos_block4.h
extract_bit_u32_MSB1.o: extract_bit_u32_MSB1.c scicos_block4.h
extract_bit_u16_MSB1.o: extract_bit_u16_MSB1.c scicos_block4.h
extract_bit_u8_MSB1.o: extract_bit_u8_MSB1.c scicos_block4.h
extract_bit_u32_RB1.o: extract_bit_u32_RB1.c scicos_block4.h
extract_bit_u16_RB1.o: extract_bit_u16_RB1.c scicos_block4.h
extract_bit_u8_RB1.o: extract_bit_u8_RB1.c scicos_block4.h 
rndblk_m.o: rndblk_m.c scicos_block4.h ../machine.h
relational_op.o: relational_op.c scicos_block4.h
curve_c.o: curve_c.c scicos_block4.h
counter.o: counter.c scicos_block4.h
m_frequ.o: m_frequ.c scicos_block4.h
tows_c.o: tows_c.c scicos_block4.h ../machine.h ../stack.h
rndblkz_m.o: rndblkz_m.c scicos_block4.h ../machine.h
fromws_c.o: fromws_c.c scicos_block4.h 
mathermit_m.o: mathermit_m.c scicos_block4.h ../machine.h

sundials/cvode.o: sundials/cvode.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/cvode_io.o: sundials/cvode_io.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/cvode_dense.o: sundials/cvode_dense.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/nvector_serial.o: sundials/nvector_serial.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/ida.o: sundials/ida.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/ida_dense.o: sundials/ida_dense.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/ida_io.o: sundials/ida_io.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/ida_ic.o: sundials/ida_ic.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/sundials_nvector.o: sundials/sundials_nvector.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/sundials_math.o: sundials/sundials_math.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/sundials_dense.o: sundials/sundials_dense.c
	$(CC) $(CFLAGS) -o $@ -c $<
sundials/sundials_smalldense.o: sundials/sundials_smalldense.c
	$(CC) $(CFLAGS) -o $@ -c $<
