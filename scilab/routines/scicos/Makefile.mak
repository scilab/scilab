#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/scicos.lib

BLOCKSF=evtdly.obj cstblk.obj \
	lusat.obj pload.obj qzcel.obj qzflr.obj \
	qzrnd.obj qztrn.obj lsplit.obj csslti.obj \
	dsslti.obj trash.obj zcross.obj \
	expblk.obj logblk.obj sinblk.obj tanblk.obj powblk.obj \
	sqrblk.obj delay.obj selblk.obj forblk.obj writef.obj invblk.obj hltblk.obj \
	gensin.obj rndblk.obj lookup.obj timblk.obj gensqr.obj mfclck.obj \
	sawtth.obj tcslti.obj tcsltj.obj integr.obj readf.obj affich2.obj affich.obj \
	intpol.obj intplt.obj minblk.obj maxblk.obj dlradp.obj  iocopy.obj \
	sum2.obj sum3.obj delayv.obj mux.obj demux.obj samphold.obj dollar.obj \
	intrp2.obj intrpl.obj fsv.obj memo.obj diffblk.obj constraint.obj \
	absblk.obj andlog.obj bidon.obj gain.obj cdummy.obj dband.obj cosblk.obj ifthel.obj \
	eselect.obj


BLOCKS_CODE=evtdly.obj cstblk.obj \
	lusat.obj pload.obj qzcel.obj qzflr.obj \
	qzrnd.obj qztrn.obj  lsplit.obj csslti.obj \
	dsslti.obj trash.obj zcross.obj \
	expblk.obj logblk.obj sinblk.obj tanblk.obj powblk.obj \
	sqrblk.obj delay.obj selblk.obj forblk.obj  writef.obj invblk.obj hltblk.obj \
	gensin.obj rndblk.obj lookup.obj timblk.obj gensqr.obj mfclck.obj \
	sawtth.obj tcslti.obj tcsltj.obj integr.obj readf.obj affich2.obj affich.obj \
	intpol.obj intplt.obj minblk.obj maxblk.obj dlradp.obj  iocopy.obj \
	sum2.obj sum3.obj delayv.obj mux.obj demux.obj samphold.obj dollar.obj \
	intrp2.obj intrpl.obj fsv.obj memo.obj \
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
	csslti4.obj cstblk4.obj samphold4.obj dollar4.obj invblk4.obj delay4.obj \
	cevscpe.obj cfscope.obj cscopxy.obj canimxy.obj canimxy3d.obj cscopxy3d.obj \
	matmul_m.obj mattran_m.obj cmatview.obj cmat3d.obj \
	extdiag.obj exttril.obj mat_bksl.obj mat_diag.obj mat_lu.obj mat_svd.obj \
	matz_absc.obj matz_conj.obj matz_expm.obj matz_reim.obj matz_svd.obj root_coef.obj \
	extdiagz.obj exttrilz.obj mat_cath.obj mat_div.obj mat_pinv.obj mat_vps.obj matz_bksl.obj \
	matz_det.obj matz_inv.obj matz_reimc.obj matz_vps.obj rootz_coef.obj extract.obj exttriu.obj \
	mat_catv.obj mat_expm.obj mat_reshape.obj mat_vpv.obj matz_cath.obj matz_diag.obj matz_lu.obj \
	matz_reshape.obj matz_vpv.obj submat.obj extractz.obj exttriuz.obj mat_det.obj mat_inv.obj \
	mat_sing.obj matz_abs.obj matz_catv.obj matz_div.obj matz_pinv.obj matz_sing.obj ricc_m.obj \
	submatz.obj switch2_m.obj dollar4_m.obj cstblk4_m.obj integralz_func.obj \
	matzmul_m.obj matztran_m.obj mat_sum.obj mat_sumc.obj mat_suml.obj cumsum_c.obj cumsum_m.obj \
	cumsum_r.obj matz_sum.obj matz_sumc.obj matz_suml.obj cumsumz_c.obj cumsumz_m.obj \
	cumsumz_r.obj selector_m.obj summation_z.obj\
	convert.obj logicalop_i32.obj logicalop_ui32.obj logicalop_i16.obj logicalop_ui16.obj \
	logicalop_i8.obj logicalop_ui8.obj logicalop_m.obj samphold4_m.obj matmul_i32s.obj \
	matmul_i32n.obj matmul_i32e.obj matmul_i16s.obj matmul_i16n.obj matmul_i16e.obj \
	matmul_i8s.obj matmul_i8n.obj matmul_i8e.obj matmul_ui32s.obj matmul_ui32n.obj \
	matmul_ui32e.obj matmul_ui16s.obj matmul_ui16n.obj matmul_ui16e.obj matmul_ui8s.obj \
	matmul_ui8n.obj matmul_ui8e.obj summation_i32s.obj summation_i32n.obj summation_i32e.obj \
	summation_i16s.obj summation_i16n.obj summation_i16e.obj summation_i8s.obj \
	summation_i8n.obj summation_i8e.obj summation_ui32s.obj summation_ui32n.obj \
	summation_ui32e.obj summation_ui16s.obj summation_ui16n.obj summation_ui16e.obj \
	summation_ui8s.obj summation_ui8n.obj summation_ui8e.obj gainblk_i32s.obj \
	gainblk_i32n.obj gainblk_i32e.obj gainblk_i16s.obj gainblk_i16n.obj gainblk_i16e.obj \
	gainblk_i8s.obj gainblk_i8n.obj gainblk_i8e.obj gainblk_ui32s.obj gainblk_ui32n.obj \
	gainblk_ui32e.obj gainblk_ui16s.obj gainblk_ui16n.obj gainblk_ui16e.obj gainblk_ui8s.obj \
	gainblk_ui8n.obj gainblk_ui8e.obj delay4_i32.obj delay4_i16.obj delay4_i8.obj delay4_ui32.obj \
	delay4_ui16.obj delay4_ui8.obj mat_sqrt.obj \
	matz_sqrt.obj relational_op_i32.obj relational_op_ui32.obj relational_op_i16.obj \
	relational_op_ui16.obj relational_op_i8.obj relational_op_ui8.obj evtdly4.obj\
	matmul2_m.obj matzmul2_m.obj expblk_m.obj logic.obj\
	bit_clear_32.obj bit_clear_16.obj bit_clear_8.obj\
	bit_set_32.obj bit_set_16.obj bit_set_8.obj extract_bit_32_UH0.obj extract_bit_16_UH0.obj \
	extract_bit_8_UH0.obj extract_bit_32_UH1.obj extract_bit_16_UH1.obj extract_bit_8_UH1.obj \
	extract_bit_32_LH.obj extract_bit_16_LH.obj extract_bit_8_LH.obj extract_bit_32_MSB0.obj \
        extract_bit_16_MSB0.obj extract_bit_8_MSB0.obj extract_bit_32_MSB1.obj extract_bit_16_MSB1.obj \
        extract_bit_8_MSB1.obj extract_bit_32_LSB.obj extract_bit_16_LSB.obj extract_bit_8_LSB.obj \
        extract_bit_32_RB0.obj extract_bit_16_RB0.obj extract_bit_8_RB0.obj extract_bit_32_RB1.obj \
        extract_bit_16_RB1.obj extract_bit_8_RB1.obj shift_8_LA.obj shift_16_LA.obj shift_32_LA.obj \
	shift_8_LC.obj shift_16_LC.obj shift_32_LC.obj shift_8_RA.obj shift_16_RA.obj shift_32_RA.obj \
	shift_8_RC.obj shift_16_RC.obj shift_32_RC.obj shift_u8_RA.obj shift_u16_RA.obj shift_u32_RA.obj\
	extract_bit_u32_UH1.obj extract_bit_u16_UH1.obj extract_bit_u8_UH1.obj extract_bit_u32_MSB1.obj\
	extract_bit_u16_MSB1.obj extract_bit_u8_MSB1.obj extract_bit_u32_RB1.obj extract_bit_u16_RB1.obj \
        extract_bit_u8_RB1.obj rndblk_m.obj relational_op.obj curve_c.obj counter.obj m_frequ.obj \
        tows_c.obj rndblkz_m.obj fromws_c.obj  mathermit_m.obj scicosexit.obj automat.obj

OBJSF=intcos.obj coselm.obj sciblk.obj  \
	sctree.obj ftree2.obj ftree3.obj ftree4.obj skipvars.obj scierr.obj scifunc.obj \
	list2vars.obj scitoi.obj scitod.obj vvtosci.obj \
	scitovv.obj  $(BLOCKS_CODE)

OBJSC=scicos.obj intcscicos.obj import.obj realtime.obj \
	intrealtime.obj matscicos.obj blocks.obj scicosclip.obj \
	sciblk2.obj sciblk2i.obj sciblk4.obj scicos_malloc.obj \
	scicos_free.obj tree.obj \
	scoGetProperty.obj scoSetProperty.obj scoMisc.obj \
	scoMemoryScope.obj scoWindowScope.obj $(BLOCKSC)\
	sundials/cvode.obj sundials/cvode_io.obj sundials/cvode_dense.obj\
	sundials/nvector_serial.obj sundials/ida.obj sundials/ida_dense.obj\
	sundials/ida_io.obj sundials/ida_ic.obj \
	sundials/sundials_nvector.obj sundials/sundials_math.obj\
	sundials/sundials_dense.obj sundials/sundials_smalldense.obj

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
cscope.obj: cscope.c scicos_block.h ../machine.h
deadband.obj: deadband.c scicos_block.h
deriv.obj: deriv.c scicos_block.h
extractor.obj: extractor.c scicos_block.h
gainblk.obj: gainblk.c scicos_block.h 
evaluate_expr.obj:  scicos_block.h
hystheresis.obj: hystheresis.c scicos_block.h
import.obj: import.c ../machine.h import.h
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
matmul_m.obj: matmul_m.c scicos_block4.h
mattram_m.obj: mattran_m.c scicos_block4.h
