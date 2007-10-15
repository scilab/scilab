#ifndef __SCICOS_BLOCKS__ 
#define __SCICOS_BLOCKS__ 
/******* Copyright INRIA *************/
/******* Please do not edit *************/
extern void F2C(evtdly) __PARAMS((ARGS_scicos));
extern void F2C(cstblk) __PARAMS((ARGS_scicos));
extern void F2C(lusat) __PARAMS((ARGS_scicos));
extern void F2C(pload) __PARAMS((ARGS_scicos));
extern void F2C(qzcel) __PARAMS((ARGS_scicos));
extern void F2C(qzflr) __PARAMS((ARGS_scicos));
extern void F2C(qzrnd) __PARAMS((ARGS_scicos));
extern void F2C(qztrn) __PARAMS((ARGS_scicos));
extern void F2C(lsplit) __PARAMS((ARGS_scicos));
extern void F2C(csslti) __PARAMS((ARGS_scicos));
extern void F2C(dsslti) __PARAMS((ARGS_scicos));
extern void F2C(trash) __PARAMS((ARGS_scicos));
extern void F2C(zcross) __PARAMS((ARGS_scicos));
extern void F2C(expblk) __PARAMS((ARGS_scicos));
extern void F2C(logblk) __PARAMS((ARGS_scicos));
extern void F2C(sinblk) __PARAMS((ARGS_scicos));
extern void F2C(tanblk) __PARAMS((ARGS_scicos));
extern void F2C(powblk) __PARAMS((ARGS_scicos));
extern void F2C(sqrblk) __PARAMS((ARGS_scicos));
extern void F2C(delay) __PARAMS((ARGS_scicos));
extern void F2C(selblk) __PARAMS((ARGS_scicos));
extern void F2C(forblk) __PARAMS((ARGS_scicos));
extern void F2C(writef) __PARAMS((ARGS_scicos));
extern void F2C(invblk) __PARAMS((ARGS_scicos));
extern void F2C(hltblk) __PARAMS((ARGS_scicos));
extern void F2C(gensin) __PARAMS((ARGS_scicos));
extern void F2C(rndblk) __PARAMS((ARGS_scicos));
extern void F2C(lookup) __PARAMS((ARGS_scicos));
extern void F2C(timblk) __PARAMS((ARGS_scicos));
extern void F2C(gensqr) __PARAMS((ARGS_scicos));
extern void F2C(mfclck) __PARAMS((ARGS_scicos));
extern void F2C(sawtth) __PARAMS((ARGS_scicos));
extern void F2C(tcslti) __PARAMS((ARGS_scicos));
extern void F2C(tcsltj) __PARAMS((ARGS_scicos));
extern void F2C(integr) __PARAMS((ARGS_scicos));
extern void F2C(readf) __PARAMS((ARGS_scicos));
extern void F2C(affich2) __PARAMS((ARGS_scicos));
extern void F2C(affich) __PARAMS((ARGS_scicos));
extern void F2C(intpol) __PARAMS((ARGS_scicos));
extern void F2C(intplt) __PARAMS((ARGS_scicos));
extern void F2C(minblk) __PARAMS((ARGS_scicos));
extern void F2C(maxblk) __PARAMS((ARGS_scicos));
extern void F2C(dlradp) __PARAMS((ARGS_scicos));
extern void F2C(iocopy) __PARAMS((ARGS_scicos));
extern void F2C(sum2) __PARAMS((ARGS_scicos));
extern void F2C(sum3) __PARAMS((ARGS_scicos));
extern void F2C(delayv) __PARAMS((ARGS_scicos));
extern void F2C(mux) __PARAMS((ARGS_scicos));
extern void F2C(demux) __PARAMS((ARGS_scicos));
extern void F2C(samphold) __PARAMS((ARGS_scicos));
extern void F2C(dollar) __PARAMS((ARGS_scicos));
extern void F2C(intrp2) __PARAMS((ARGS_scicos));
extern void F2C(intrpl) __PARAMS((ARGS_scicos));
extern void F2C(fsv) __PARAMS((ARGS_scicos));
extern void F2C(memo) __PARAMS((ARGS_scicos));
extern void F2C(diffblk) __PARAMS((ARGS_scicos));
extern void F2C(constraint) __PARAMS((ARGS_scicos));
extern void F2C(absblk) __PARAMS((ARGS_scicos));
extern void F2C(andlog) __PARAMS((ARGS_scicos));
extern void F2C(bidon) __PARAMS((ARGS_scicos));
extern void F2C(gain) __PARAMS((ARGS_scicos));
extern void F2C(cdummy) __PARAMS((ARGS_scicos));
extern void F2C(dband) __PARAMS((ARGS_scicos));
extern void F2C(cosblk) __PARAMS((ARGS_scicos));
extern void F2C(ifthel) __PARAMS((ARGS_scicos));
extern void F2C(eselect) __PARAMS((ARGS_scicos));
extern void selector __PARAMS((ARGS_scicos));
extern void sum __PARAMS((ARGS_scicos));
extern void prod __PARAMS((ARGS_scicos));
extern void switchn __PARAMS((ARGS_scicos));
extern void relay __PARAMS((ARGS_scicos));
extern void readc __PARAMS((ARGS_scicos));
extern void writec __PARAMS((ARGS_scicos));
extern void writeau __PARAMS((ARGS_scicos));
extern void readau __PARAMS((ARGS_scicos));
extern void plusblk __PARAMS((ARGS_scicos));
extern void slider __PARAMS((ARGS_scicos));
extern void zcross2 __PARAMS((ARGS_scicos));
extern void mswitch __PARAMS((ARGS_scicos));
extern void logicalop __PARAMS((ARGS_scicos));
extern void switch2 __PARAMS((ARGS_scicos));
extern void variable_delay __PARAMS((ARGS_scicos));
extern void time_delay __PARAMS((ARGS_scicos));
extern void cscope __PARAMS((ARGS_scicos));
extern void cmscope __PARAMS((ARGS_scicos));
extern void satur __PARAMS((ARGS_scicos));
extern void step_func __PARAMS((ARGS_scicos));
extern void integral_func __PARAMS((ARGS_scicos));
extern void absolute_value __PARAMS((ARGS_scicos));
extern void bounce_ball __PARAMS((ARGS_scicos));
extern void bouncexy __PARAMS((ARGS_scicos));
extern void extractor __PARAMS((ARGS_scicos));
extern void scalar2vector __PARAMS((ARGS_scicos));
extern void minmax __PARAMS((ARGS_scicos));
extern void signum __PARAMS((ARGS_scicos));
extern void product __PARAMS((ARGS_scicos));
extern void summation __PARAMS((ARGS_scicos));
extern void multiplex __PARAMS((ARGS_scicos));
extern void gainblk __PARAMS((ARGS_scicos));
extern void relationalop __PARAMS((ARGS_scicos));
extern void modulo_count __PARAMS((ARGS_scicos));
extern void hystheresis __PARAMS((ARGS_scicos));
extern void ratelimiter __PARAMS((ARGS_scicos));
extern void backlash __PARAMS((ARGS_scicos));
extern void deadband __PARAMS((ARGS_scicos));
extern void ramp __PARAMS((ARGS_scicos));
extern void evaluate_expr __PARAMS((ARGS_scicos));
extern void deriv __PARAMS((ARGS_scicos));
extern void sin_blk __PARAMS((ARGS_scicos));
extern void cos_blk __PARAMS((ARGS_scicos));
extern void tan_blk __PARAMS((ARGS_scicos));
extern void asin_blk __PARAMS((ARGS_scicos));
extern void acos_blk __PARAMS((ARGS_scicos));
extern void atan_blk __PARAMS((ARGS_scicos));
extern void sinh_blk __PARAMS((ARGS_scicos));
extern void cosh_blk __PARAMS((ARGS_scicos));
extern void tanh_blk __PARAMS((ARGS_scicos));
extern void asinh_blk __PARAMS((ARGS_scicos));
extern void acosh_blk __PARAMS((ARGS_scicos));
extern void atanh_blk __PARAMS((ARGS_scicos));
extern void evtvardly __PARAMS((ARGS_scicos));
extern void edgetrig __PARAMS((ARGS_scicos));
extern void tcslti4 __PARAMS((ARGS_scicos));
extern void tcsltj4 __PARAMS((ARGS_scicos));
extern void dsslti4 __PARAMS((ARGS_scicos));
extern void csslti4 __PARAMS((ARGS_scicos));
extern void cstblk4 __PARAMS((ARGS_scicos));
extern void samphold4 __PARAMS((ARGS_scicos));
extern void dollar4 __PARAMS((ARGS_scicos));
extern void invblk4 __PARAMS((ARGS_scicos));
extern void delay4 __PARAMS((ARGS_scicos));
extern void cevscpe __PARAMS((ARGS_scicos));
extern void cfscope __PARAMS((ARGS_scicos));
extern void cscopxy __PARAMS((ARGS_scicos));
extern void canimxy __PARAMS((ARGS_scicos));
extern void canimxy3d __PARAMS((ARGS_scicos));
extern void cscopxy3d __PARAMS((ARGS_scicos));
extern void matmul_m __PARAMS((ARGS_scicos));
extern void mattran_m __PARAMS((ARGS_scicos));
extern void cmatview __PARAMS((ARGS_scicos));
extern void cmat3d __PARAMS((ARGS_scicos));
extern void extdiag __PARAMS((ARGS_scicos));
extern void exttril __PARAMS((ARGS_scicos));
extern void mat_bksl __PARAMS((ARGS_scicos));
extern void mat_diag __PARAMS((ARGS_scicos));
extern void mat_lu __PARAMS((ARGS_scicos));
extern void mat_svd __PARAMS((ARGS_scicos));
extern void matz_absc __PARAMS((ARGS_scicos));
extern void matz_conj __PARAMS((ARGS_scicos));
extern void matz_expm __PARAMS((ARGS_scicos));
extern void matz_reim __PARAMS((ARGS_scicos));
extern void matz_svd __PARAMS((ARGS_scicos));
extern void root_coef __PARAMS((ARGS_scicos));
extern void extdiagz __PARAMS((ARGS_scicos));
extern void exttrilz __PARAMS((ARGS_scicos));
extern void mat_cath __PARAMS((ARGS_scicos));
extern void mat_div __PARAMS((ARGS_scicos));
extern void mat_pinv __PARAMS((ARGS_scicos));
extern void mat_vps __PARAMS((ARGS_scicos));
extern void matz_bksl __PARAMS((ARGS_scicos));
extern void matz_det __PARAMS((ARGS_scicos));
extern void matz_inv __PARAMS((ARGS_scicos));
extern void matz_reimc __PARAMS((ARGS_scicos));
extern void matz_vps __PARAMS((ARGS_scicos));
extern void rootz_coef __PARAMS((ARGS_scicos));
extern void extract __PARAMS((ARGS_scicos));
extern void exttriu __PARAMS((ARGS_scicos));
extern void mat_catv __PARAMS((ARGS_scicos));
extern void mat_expm __PARAMS((ARGS_scicos));
extern void mat_reshape __PARAMS((ARGS_scicos));
extern void mat_vpv __PARAMS((ARGS_scicos));
extern void matz_cath __PARAMS((ARGS_scicos));
extern void matz_diag __PARAMS((ARGS_scicos));
extern void matz_lu __PARAMS((ARGS_scicos));
extern void matz_reshape __PARAMS((ARGS_scicos));
extern void matz_vpv __PARAMS((ARGS_scicos));
extern void submat __PARAMS((ARGS_scicos));
extern void extractz __PARAMS((ARGS_scicos));
extern void exttriuz __PARAMS((ARGS_scicos));
extern void mat_det __PARAMS((ARGS_scicos));
extern void mat_inv __PARAMS((ARGS_scicos));
extern void mat_sing __PARAMS((ARGS_scicos));
extern void matz_abs __PARAMS((ARGS_scicos));
extern void matz_catv __PARAMS((ARGS_scicos));
extern void matz_div __PARAMS((ARGS_scicos));
extern void matz_pinv __PARAMS((ARGS_scicos));
extern void matz_sing __PARAMS((ARGS_scicos));
extern void ricc_m __PARAMS((ARGS_scicos));
extern void submatz __PARAMS((ARGS_scicos));
extern void switch2_m __PARAMS((ARGS_scicos));
extern void dollar4_m __PARAMS((ARGS_scicos));
extern void cstblk4_m __PARAMS((ARGS_scicos));
extern void integralz_func __PARAMS((ARGS_scicos));
extern void matzmul_m __PARAMS((ARGS_scicos));
extern void matztran_m __PARAMS((ARGS_scicos));
extern void mat_sum __PARAMS((ARGS_scicos));
extern void mat_sumc __PARAMS((ARGS_scicos));
extern void mat_suml __PARAMS((ARGS_scicos));
extern void cumsum_c __PARAMS((ARGS_scicos));
extern void cumsum_m __PARAMS((ARGS_scicos));
extern void cumsum_r __PARAMS((ARGS_scicos));
extern void matz_sum __PARAMS((ARGS_scicos));
extern void matz_sumc __PARAMS((ARGS_scicos));
extern void matz_suml __PARAMS((ARGS_scicos));
extern void cumsumz_c __PARAMS((ARGS_scicos));
extern void cumsumz_m __PARAMS((ARGS_scicos));
extern void cumsumz_r __PARAMS((ARGS_scicos));
extern void selector_m __PARAMS((ARGS_scicos));
extern void summation_z __PARAMS((ARGS_scicos));
extern void convert __PARAMS((ARGS_scicos));
extern void logicalop_i32 __PARAMS((ARGS_scicos));
extern void logicalop_ui32 __PARAMS((ARGS_scicos));
extern void logicalop_i16 __PARAMS((ARGS_scicos));
extern void logicalop_ui16 __PARAMS((ARGS_scicos));
extern void logicalop_i8 __PARAMS((ARGS_scicos));
extern void logicalop_ui8 __PARAMS((ARGS_scicos));
extern void logicalop_m __PARAMS((ARGS_scicos));
extern void samphold4_m __PARAMS((ARGS_scicos));
extern void matmul_i32s __PARAMS((ARGS_scicos));
extern void matmul_i32n __PARAMS((ARGS_scicos));
extern void matmul_i32e __PARAMS((ARGS_scicos));
extern void matmul_i16s __PARAMS((ARGS_scicos));
extern void matmul_i16n __PARAMS((ARGS_scicos));
extern void matmul_i16e __PARAMS((ARGS_scicos));
extern void matmul_i8s __PARAMS((ARGS_scicos));
extern void matmul_i8n __PARAMS((ARGS_scicos));
extern void matmul_i8e __PARAMS((ARGS_scicos));
extern void matmul_ui32s __PARAMS((ARGS_scicos));
extern void matmul_ui32n __PARAMS((ARGS_scicos));
extern void matmul_ui32e __PARAMS((ARGS_scicos));
extern void matmul_ui16s __PARAMS((ARGS_scicos));
extern void matmul_ui16n __PARAMS((ARGS_scicos));
extern void matmul_ui16e __PARAMS((ARGS_scicos));
extern void matmul_ui8s __PARAMS((ARGS_scicos));
extern void matmul_ui8n __PARAMS((ARGS_scicos));
extern void matmul_ui8e __PARAMS((ARGS_scicos));
extern void summation_i32s __PARAMS((ARGS_scicos));
extern void summation_i32n __PARAMS((ARGS_scicos));
extern void summation_i32e __PARAMS((ARGS_scicos));
extern void summation_i16s __PARAMS((ARGS_scicos));
extern void summation_i16n __PARAMS((ARGS_scicos));
extern void summation_i16e __PARAMS((ARGS_scicos));
extern void summation_i8s __PARAMS((ARGS_scicos));
extern void summation_i8n __PARAMS((ARGS_scicos));
extern void summation_i8e __PARAMS((ARGS_scicos));
extern void summation_ui32s __PARAMS((ARGS_scicos));
extern void summation_ui32n __PARAMS((ARGS_scicos));
extern void summation_ui32e __PARAMS((ARGS_scicos));
extern void summation_ui16s __PARAMS((ARGS_scicos));
extern void summation_ui16n __PARAMS((ARGS_scicos));
extern void summation_ui16e __PARAMS((ARGS_scicos));
extern void summation_ui8s __PARAMS((ARGS_scicos));
extern void summation_ui8n __PARAMS((ARGS_scicos));
extern void summation_ui8e __PARAMS((ARGS_scicos));
extern void gainblk_i32s __PARAMS((ARGS_scicos));
extern void gainblk_i32n __PARAMS((ARGS_scicos));
extern void gainblk_i32e __PARAMS((ARGS_scicos));
extern void gainblk_i16s __PARAMS((ARGS_scicos));
extern void gainblk_i16n __PARAMS((ARGS_scicos));
extern void gainblk_i16e __PARAMS((ARGS_scicos));
extern void gainblk_i8s __PARAMS((ARGS_scicos));
extern void gainblk_i8n __PARAMS((ARGS_scicos));
extern void gainblk_i8e __PARAMS((ARGS_scicos));
extern void gainblk_ui32s __PARAMS((ARGS_scicos));
extern void gainblk_ui32n __PARAMS((ARGS_scicos));
extern void gainblk_ui32e __PARAMS((ARGS_scicos));
extern void gainblk_ui16s __PARAMS((ARGS_scicos));
extern void gainblk_ui16n __PARAMS((ARGS_scicos));
extern void gainblk_ui16e __PARAMS((ARGS_scicos));
extern void gainblk_ui8s __PARAMS((ARGS_scicos));
extern void gainblk_ui8n __PARAMS((ARGS_scicos));
extern void gainblk_ui8e __PARAMS((ARGS_scicos));
extern void delay4_i32 __PARAMS((ARGS_scicos));
extern void delay4_i16 __PARAMS((ARGS_scicos));
extern void delay4_i8 __PARAMS((ARGS_scicos));
extern void delay4_ui32 __PARAMS((ARGS_scicos));
extern void delay4_ui16 __PARAMS((ARGS_scicos));
extern void delay4_ui8 __PARAMS((ARGS_scicos));
extern void mat_sqrt __PARAMS((ARGS_scicos));
extern void matz_sqrt __PARAMS((ARGS_scicos));
extern void relational_op_i32 __PARAMS((ARGS_scicos));
extern void relational_op_ui32 __PARAMS((ARGS_scicos));
extern void relational_op_i16 __PARAMS((ARGS_scicos));
extern void relational_op_ui16 __PARAMS((ARGS_scicos));
extern void relational_op_i8 __PARAMS((ARGS_scicos));
extern void relational_op_ui8 __PARAMS((ARGS_scicos));
extern void evtdly4 __PARAMS((ARGS_scicos));
extern void matmul2_m __PARAMS((ARGS_scicos));
extern void matzmul2_m __PARAMS((ARGS_scicos));
extern void expblk_m __PARAMS((ARGS_scicos));
extern void logic __PARAMS((ARGS_scicos));
extern void bit_clear_32 __PARAMS((ARGS_scicos));
extern void bit_clear_16 __PARAMS((ARGS_scicos));
extern void bit_clear_8 __PARAMS((ARGS_scicos));
extern void bit_set_32 __PARAMS((ARGS_scicos));
extern void bit_set_16 __PARAMS((ARGS_scicos));
extern void bit_set_8 __PARAMS((ARGS_scicos));
extern void extract_bit_32_UH0 __PARAMS((ARGS_scicos));
extern void extract_bit_16_UH0 __PARAMS((ARGS_scicos));
extern void extract_bit_8_UH0 __PARAMS((ARGS_scicos));
extern void extract_bit_32_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_16_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_8_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_32_LH __PARAMS((ARGS_scicos));
extern void extract_bit_16_LH __PARAMS((ARGS_scicos));
extern void extract_bit_8_LH __PARAMS((ARGS_scicos));
extern void extract_bit_32_MSB0 __PARAMS((ARGS_scicos));
extern void extract_bit_16_MSB0 __PARAMS((ARGS_scicos));
extern void extract_bit_8_MSB0 __PARAMS((ARGS_scicos));
extern void extract_bit_32_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_16_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_8_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_32_LSB __PARAMS((ARGS_scicos));
extern void extract_bit_16_LSB __PARAMS((ARGS_scicos));
extern void extract_bit_8_LSB __PARAMS((ARGS_scicos));
extern void extract_bit_32_RB0 __PARAMS((ARGS_scicos));
extern void extract_bit_16_RB0 __PARAMS((ARGS_scicos));
extern void extract_bit_8_RB0 __PARAMS((ARGS_scicos));
extern void extract_bit_32_RB1 __PARAMS((ARGS_scicos));
extern void extract_bit_16_RB1 __PARAMS((ARGS_scicos));
extern void extract_bit_8_RB1 __PARAMS((ARGS_scicos));
extern void shift_8_LA __PARAMS((ARGS_scicos));
extern void shift_16_LA __PARAMS((ARGS_scicos));
extern void shift_32_LA __PARAMS((ARGS_scicos));
extern void shift_8_LC __PARAMS((ARGS_scicos));
extern void shift_16_LC __PARAMS((ARGS_scicos));
extern void shift_32_LC __PARAMS((ARGS_scicos));
extern void shift_8_RA __PARAMS((ARGS_scicos));
extern void shift_16_RA __PARAMS((ARGS_scicos));
extern void shift_32_RA __PARAMS((ARGS_scicos));
extern void shift_8_RC __PARAMS((ARGS_scicos));
extern void shift_16_RC __PARAMS((ARGS_scicos));
extern void shift_32_RC __PARAMS((ARGS_scicos));
extern void shift_u8_RA __PARAMS((ARGS_scicos));
extern void shift_u16_RA __PARAMS((ARGS_scicos));
extern void shift_u32_RA __PARAMS((ARGS_scicos));
extern void extract_bit_u32_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_u16_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_u8_UH1 __PARAMS((ARGS_scicos));
extern void extract_bit_u32_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_u16_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_u8_MSB1 __PARAMS((ARGS_scicos));
extern void extract_bit_u32_RB1 __PARAMS((ARGS_scicos));
extern void extract_bit_u16_RB1 __PARAMS((ARGS_scicos));
extern void extract_bit_u8_RB1 __PARAMS((ARGS_scicos));
extern void rndblk_m __PARAMS((ARGS_scicos));
extern void relational_op __PARAMS((ARGS_scicos));
extern void curve_c __PARAMS((ARGS_scicos));
extern void counter __PARAMS((ARGS_scicos));
extern void m_frequ __PARAMS((ARGS_scicos));
extern void tows_c __PARAMS((ARGS_scicos));
extern void rndblkz_m __PARAMS((ARGS_scicos));
extern void fromws_c __PARAMS((ARGS_scicos));
extern void mathermit_m __PARAMS((ARGS_scicos));
extern void scicosexit __PARAMS((ARGS_scicos));
extern void automat __PARAMS((ARGS_scicos));
 
OpTab tabsim[] ={
{"absblk",(ScicosF) F2C(absblk)},
{"absolute_value",(ScicosF) absolute_value},
{"acos_blk",(ScicosF) acos_blk},
{"acosh_blk",(ScicosF) acosh_blk},
{"affich2",(ScicosF) F2C(affich2)},
{"affich",(ScicosF) F2C(affich)},
{"andlog",(ScicosF) F2C(andlog)},
{"asin_blk",(ScicosF) asin_blk},
{"asinh_blk",(ScicosF) asinh_blk},
{"atan_blk",(ScicosF) atan_blk},
{"atanh_blk",(ScicosF) atanh_blk},
{"automat",(ScicosF) automat},
{"backlash",(ScicosF) backlash},
{"bidon",(ScicosF) F2C(bidon)},
{"bit_clear_16",(ScicosF) bit_clear_16},
{"bit_clear_32",(ScicosF) bit_clear_32},
{"bit_clear_8",(ScicosF) bit_clear_8},
{"bit_set_16",(ScicosF) bit_set_16},
{"bit_set_32",(ScicosF) bit_set_32},
{"bit_set_8",(ScicosF) bit_set_8},
{"bounce_ball",(ScicosF) bounce_ball},
{"bouncexy",(ScicosF) bouncexy},
{"canimxy3d",(ScicosF) canimxy3d},
{"canimxy",(ScicosF) canimxy},
{"cdummy",(ScicosF) F2C(cdummy)},
{"cevscpe",(ScicosF) cevscpe},
{"cfscope",(ScicosF) cfscope},
{"cmat3d",(ScicosF) cmat3d},
{"cmatview",(ScicosF) cmatview},
{"cmscope",(ScicosF) cmscope},
{"constraint",(ScicosF) F2C(constraint)},
{"convert",(ScicosF) convert},
{"cos_blk",(ScicosF) cos_blk},
{"cosblk",(ScicosF) F2C(cosblk)},
{"cosh_blk",(ScicosF) cosh_blk},
{"counter",(ScicosF) counter},
{"cscope",(ScicosF) cscope},
{"cscopxy3d",(ScicosF) cscopxy3d},
{"cscopxy",(ScicosF) cscopxy},
{"csslti4",(ScicosF) csslti4},
{"csslti",(ScicosF) F2C(csslti)},
{"cstblk4_m",(ScicosF) cstblk4_m},
{"cstblk4",(ScicosF) cstblk4},
{"cstblk",(ScicosF) F2C(cstblk)},
{"cumsum_c",(ScicosF) cumsum_c},
{"cumsum_m",(ScicosF) cumsum_m},
{"cumsum_r",(ScicosF) cumsum_r},
{"cumsumz_c",(ScicosF) cumsumz_c},
{"cumsumz_m",(ScicosF) cumsumz_m},
{"cumsumz_r",(ScicosF) cumsumz_r},
{"curve_c",(ScicosF) curve_c},
{"dband",(ScicosF) F2C(dband)},
{"deadband",(ScicosF) deadband},
{"delay4_i16",(ScicosF) delay4_i16},
{"delay4_i32",(ScicosF) delay4_i32},
{"delay4_i8",(ScicosF) delay4_i8},
{"delay4",(ScicosF) delay4},
{"delay4_ui16",(ScicosF) delay4_ui16},
{"delay4_ui32",(ScicosF) delay4_ui32},
{"delay4_ui8",(ScicosF) delay4_ui8},
{"delay",(ScicosF) F2C(delay)},
{"delayv",(ScicosF) F2C(delayv)},
{"demux",(ScicosF) F2C(demux)},
{"deriv",(ScicosF) deriv},
{"diffblk",(ScicosF) F2C(diffblk)},
{"dlradp",(ScicosF) F2C(dlradp)},
{"dollar4_m",(ScicosF) dollar4_m},
{"dollar4",(ScicosF) dollar4},
{"dollar",(ScicosF) F2C(dollar)},
{"dsslti4",(ScicosF) dsslti4},
{"dsslti",(ScicosF) F2C(dsslti)},
{"edgetrig",(ScicosF) edgetrig},
{"eselect",(ScicosF) F2C(eselect)},
{"evaluate_expr",(ScicosF) evaluate_expr},
{"evtdly4",(ScicosF) evtdly4},
{"evtdly",(ScicosF) F2C(evtdly)},
{"evtvardly",(ScicosF) evtvardly},
{"expblk_m",(ScicosF) expblk_m},
{"expblk",(ScicosF) F2C(expblk)},
{"extdiag",(ScicosF) extdiag},
{"extdiagz",(ScicosF) extdiagz},
{"extract_bit_16_LH",(ScicosF) extract_bit_16_LH},
{"extract_bit_16_LSB",(ScicosF) extract_bit_16_LSB},
{"extract_bit_16_MSB0",(ScicosF) extract_bit_16_MSB0},
{"extract_bit_16_MSB1",(ScicosF) extract_bit_16_MSB1},
{"extract_bit_16_RB0",(ScicosF) extract_bit_16_RB0},
{"extract_bit_16_RB1",(ScicosF) extract_bit_16_RB1},
{"extract_bit_16_UH0",(ScicosF) extract_bit_16_UH0},
{"extract_bit_16_UH1",(ScicosF) extract_bit_16_UH1},
{"extract_bit_32_LH",(ScicosF) extract_bit_32_LH},
{"extract_bit_32_LSB",(ScicosF) extract_bit_32_LSB},
{"extract_bit_32_MSB0",(ScicosF) extract_bit_32_MSB0},
{"extract_bit_32_MSB1",(ScicosF) extract_bit_32_MSB1},
{"extract_bit_32_RB0",(ScicosF) extract_bit_32_RB0},
{"extract_bit_32_RB1",(ScicosF) extract_bit_32_RB1},
{"extract_bit_32_UH0",(ScicosF) extract_bit_32_UH0},
{"extract_bit_32_UH1",(ScicosF) extract_bit_32_UH1},
{"extract_bit_8_LH",(ScicosF) extract_bit_8_LH},
{"extract_bit_8_LSB",(ScicosF) extract_bit_8_LSB},
{"extract_bit_8_MSB0",(ScicosF) extract_bit_8_MSB0},
{"extract_bit_8_MSB1",(ScicosF) extract_bit_8_MSB1},
{"extract_bit_8_RB0",(ScicosF) extract_bit_8_RB0},
{"extract_bit_8_RB1",(ScicosF) extract_bit_8_RB1},
{"extract_bit_8_UH0",(ScicosF) extract_bit_8_UH0},
{"extract_bit_8_UH1",(ScicosF) extract_bit_8_UH1},
{"extract_bit_u16_MSB1",(ScicosF) extract_bit_u16_MSB1},
{"extract_bit_u16_RB1",(ScicosF) extract_bit_u16_RB1},
{"extract_bit_u16_UH1",(ScicosF) extract_bit_u16_UH1},
{"extract_bit_u32_MSB1",(ScicosF) extract_bit_u32_MSB1},
{"extract_bit_u32_RB1",(ScicosF) extract_bit_u32_RB1},
{"extract_bit_u32_UH1",(ScicosF) extract_bit_u32_UH1},
{"extract_bit_u8_MSB1",(ScicosF) extract_bit_u8_MSB1},
{"extract_bit_u8_RB1",(ScicosF) extract_bit_u8_RB1},
{"extract_bit_u8_UH1",(ScicosF) extract_bit_u8_UH1},
{"extractor",(ScicosF) extractor},
{"extract",(ScicosF) extract},
{"extractz",(ScicosF) extractz},
{"exttril",(ScicosF) exttril},
{"exttrilz",(ScicosF) exttrilz},
{"exttriu",(ScicosF) exttriu},
{"exttriuz",(ScicosF) exttriuz},
{"forblk",(ScicosF) F2C(forblk)},
{"fromws_c",(ScicosF) fromws_c},
{"fsv",(ScicosF) F2C(fsv)},
{"gainblk_i16e",(ScicosF) gainblk_i16e},
{"gainblk_i16n",(ScicosF) gainblk_i16n},
{"gainblk_i16s",(ScicosF) gainblk_i16s},
{"gainblk_i32e",(ScicosF) gainblk_i32e},
{"gainblk_i32n",(ScicosF) gainblk_i32n},
{"gainblk_i32s",(ScicosF) gainblk_i32s},
{"gainblk_i8e",(ScicosF) gainblk_i8e},
{"gainblk_i8n",(ScicosF) gainblk_i8n},
{"gainblk_i8s",(ScicosF) gainblk_i8s},
{"gainblk",(ScicosF) gainblk},
{"gainblk_ui16e",(ScicosF) gainblk_ui16e},
{"gainblk_ui16n",(ScicosF) gainblk_ui16n},
{"gainblk_ui16s",(ScicosF) gainblk_ui16s},
{"gainblk_ui32e",(ScicosF) gainblk_ui32e},
{"gainblk_ui32n",(ScicosF) gainblk_ui32n},
{"gainblk_ui32s",(ScicosF) gainblk_ui32s},
{"gainblk_ui8e",(ScicosF) gainblk_ui8e},
{"gainblk_ui8n",(ScicosF) gainblk_ui8n},
{"gainblk_ui8s",(ScicosF) gainblk_ui8s},
{"gain",(ScicosF) F2C(gain)},
{"gensin",(ScicosF) F2C(gensin)},
{"gensqr",(ScicosF) F2C(gensqr)},
{"hltblk",(ScicosF) F2C(hltblk)},
{"hystheresis",(ScicosF) hystheresis},
{"ifthel",(ScicosF) F2C(ifthel)},
{"integral_func",(ScicosF) integral_func},
{"integralz_func",(ScicosF) integralz_func},
{"integr",(ScicosF) F2C(integr)},
{"intplt",(ScicosF) F2C(intplt)},
{"intpol",(ScicosF) F2C(intpol)},
{"intrp2",(ScicosF) F2C(intrp2)},
{"intrpl",(ScicosF) F2C(intrpl)},
{"invblk4",(ScicosF) invblk4},
{"invblk",(ScicosF) F2C(invblk)},
{"iocopy",(ScicosF) F2C(iocopy)},
{"logblk",(ScicosF) F2C(logblk)},
{"logicalop_i16",(ScicosF) logicalop_i16},
{"logicalop_i32",(ScicosF) logicalop_i32},
{"logicalop_i8",(ScicosF) logicalop_i8},
{"logicalop_m",(ScicosF) logicalop_m},
{"logicalop",(ScicosF) logicalop},
{"logicalop_ui16",(ScicosF) logicalop_ui16},
{"logicalop_ui32",(ScicosF) logicalop_ui32},
{"logicalop_ui8",(ScicosF) logicalop_ui8},
{"logic",(ScicosF) logic},
{"lookup",(ScicosF) F2C(lookup)},
{"lsplit",(ScicosF) F2C(lsplit)},
{"lusat",(ScicosF) F2C(lusat)},
{"mat_bksl",(ScicosF) mat_bksl},
{"mat_cath",(ScicosF) mat_cath},
{"mat_catv",(ScicosF) mat_catv},
{"mat_det",(ScicosF) mat_det},
{"mat_diag",(ScicosF) mat_diag},
{"mat_div",(ScicosF) mat_div},
{"mat_expm",(ScicosF) mat_expm},
{"mathermit_m",(ScicosF) mathermit_m},
{"mat_inv",(ScicosF) mat_inv},
{"mat_lu",(ScicosF) mat_lu},
{"matmul2_m",(ScicosF) matmul2_m},
{"matmul_i16e",(ScicosF) matmul_i16e},
{"matmul_i16n",(ScicosF) matmul_i16n},
{"matmul_i16s",(ScicosF) matmul_i16s},
{"matmul_i32e",(ScicosF) matmul_i32e},
{"matmul_i32n",(ScicosF) matmul_i32n},
{"matmul_i32s",(ScicosF) matmul_i32s},
{"matmul_i8e",(ScicosF) matmul_i8e},
{"matmul_i8n",(ScicosF) matmul_i8n},
{"matmul_i8s",(ScicosF) matmul_i8s},
{"matmul_m",(ScicosF) matmul_m},
{"matmul_ui16e",(ScicosF) matmul_ui16e},
{"matmul_ui16n",(ScicosF) matmul_ui16n},
{"matmul_ui16s",(ScicosF) matmul_ui16s},
{"matmul_ui32e",(ScicosF) matmul_ui32e},
{"matmul_ui32n",(ScicosF) matmul_ui32n},
{"matmul_ui32s",(ScicosF) matmul_ui32s},
{"matmul_ui8e",(ScicosF) matmul_ui8e},
{"matmul_ui8n",(ScicosF) matmul_ui8n},
{"matmul_ui8s",(ScicosF) matmul_ui8s},
{"mat_pinv",(ScicosF) mat_pinv},
{"mat_reshape",(ScicosF) mat_reshape},
{"mat_sing",(ScicosF) mat_sing},
{"mat_sqrt",(ScicosF) mat_sqrt},
{"mat_sumc",(ScicosF) mat_sumc},
{"mat_suml",(ScicosF) mat_suml},
{"mat_sum",(ScicosF) mat_sum},
{"mat_svd",(ScicosF) mat_svd},
{"mattran_m",(ScicosF) mattran_m},
{"mat_vps",(ScicosF) mat_vps},
{"mat_vpv",(ScicosF) mat_vpv},
{"matz_absc",(ScicosF) matz_absc},
{"matz_abs",(ScicosF) matz_abs},
{"matz_bksl",(ScicosF) matz_bksl},
{"matz_cath",(ScicosF) matz_cath},
{"matz_catv",(ScicosF) matz_catv},
{"matz_conj",(ScicosF) matz_conj},
{"matz_det",(ScicosF) matz_det},
{"matz_diag",(ScicosF) matz_diag},
{"matz_div",(ScicosF) matz_div},
{"matz_expm",(ScicosF) matz_expm},
{"matz_inv",(ScicosF) matz_inv},
{"matz_lu",(ScicosF) matz_lu},
{"matzmul2_m",(ScicosF) matzmul2_m},
{"matzmul_m",(ScicosF) matzmul_m},
{"matz_pinv",(ScicosF) matz_pinv},
{"matz_reimc",(ScicosF) matz_reimc},
{"matz_reim",(ScicosF) matz_reim},
{"matz_reshape",(ScicosF) matz_reshape},
{"matz_sing",(ScicosF) matz_sing},
{"matz_sqrt",(ScicosF) matz_sqrt},
{"matz_sumc",(ScicosF) matz_sumc},
{"matz_suml",(ScicosF) matz_suml},
{"matz_sum",(ScicosF) matz_sum},
{"matz_svd",(ScicosF) matz_svd},
{"matztran_m",(ScicosF) matztran_m},
{"matz_vps",(ScicosF) matz_vps},
{"matz_vpv",(ScicosF) matz_vpv},
{"maxblk",(ScicosF) F2C(maxblk)},
{"memo",(ScicosF) F2C(memo)},
{"mfclck",(ScicosF) F2C(mfclck)},
{"m_frequ",(ScicosF) m_frequ},
{"minblk",(ScicosF) F2C(minblk)},
{"minmax",(ScicosF) minmax},
{"modulo_count",(ScicosF) modulo_count},
{"mswitch",(ScicosF) mswitch},
{"multiplex",(ScicosF) multiplex},
{"mux",(ScicosF) F2C(mux)},
{"pload",(ScicosF) F2C(pload)},
{"plusblk",(ScicosF) plusblk},
{"powblk",(ScicosF) F2C(powblk)},
{"prod",(ScicosF) prod},
{"product",(ScicosF) product},
{"qzcel",(ScicosF) F2C(qzcel)},
{"qzflr",(ScicosF) F2C(qzflr)},
{"qzrnd",(ScicosF) F2C(qzrnd)},
{"qztrn",(ScicosF) F2C(qztrn)},
{"ramp",(ScicosF) ramp},
{"ratelimiter",(ScicosF) ratelimiter},
{"readau",(ScicosF) readau},
{"readc",(ScicosF) readc},
{"readf",(ScicosF) F2C(readf)},
{"relational_op_i16",(ScicosF) relational_op_i16},
{"relational_op_i32",(ScicosF) relational_op_i32},
{"relational_op_i8",(ScicosF) relational_op_i8},
{"relational_op",(ScicosF) relational_op},
{"relationalop",(ScicosF) relationalop},
{"relational_op_ui16",(ScicosF) relational_op_ui16},
{"relational_op_ui32",(ScicosF) relational_op_ui32},
{"relational_op_ui8",(ScicosF) relational_op_ui8},
{"relay",(ScicosF) relay},
{"ricc_m",(ScicosF) ricc_m},
{"rndblk_m",(ScicosF) rndblk_m},
{"rndblk",(ScicosF) F2C(rndblk)},
{"rndblkz_m",(ScicosF) rndblkz_m},
{"root_coef",(ScicosF) root_coef},
{"rootz_coef",(ScicosF) rootz_coef},
{"samphold4_m",(ScicosF) samphold4_m},
{"samphold4",(ScicosF) samphold4},
{"samphold",(ScicosF) F2C(samphold)},
{"satur",(ScicosF) satur},
{"sawtth",(ScicosF) F2C(sawtth)},
{"scalar2vector",(ScicosF) scalar2vector},
{"scicosexit",(ScicosF) scicosexit},
{"selblk",(ScicosF) F2C(selblk)},
{"selector_m",(ScicosF) selector_m},
{"selector",(ScicosF) selector},
{"shift_16_LA",(ScicosF) shift_16_LA},
{"shift_16_LC",(ScicosF) shift_16_LC},
{"shift_16_RA",(ScicosF) shift_16_RA},
{"shift_16_RC",(ScicosF) shift_16_RC},
{"shift_32_LA",(ScicosF) shift_32_LA},
{"shift_32_LC",(ScicosF) shift_32_LC},
{"shift_32_RA",(ScicosF) shift_32_RA},
{"shift_32_RC",(ScicosF) shift_32_RC},
{"shift_8_LA",(ScicosF) shift_8_LA},
{"shift_8_LC",(ScicosF) shift_8_LC},
{"shift_8_RA",(ScicosF) shift_8_RA},
{"shift_8_RC",(ScicosF) shift_8_RC},
{"shift_u16_RA",(ScicosF) shift_u16_RA},
{"shift_u32_RA",(ScicosF) shift_u32_RA},
{"shift_u8_RA",(ScicosF) shift_u8_RA},
{"signum",(ScicosF) signum},
{"sinblk",(ScicosF) F2C(sinblk)},
{"sin_blk",(ScicosF) sin_blk},
{"sinh_blk",(ScicosF) sinh_blk},
{"slider",(ScicosF) slider},
{"sqrblk",(ScicosF) F2C(sqrblk)},
{"step_func",(ScicosF) step_func},
{"submat",(ScicosF) submat},
{"submatz",(ScicosF) submatz},
{"sum2",(ScicosF) F2C(sum2)},
{"sum3",(ScicosF) F2C(sum3)},
{"summation_i16e",(ScicosF) summation_i16e},
{"summation_i16n",(ScicosF) summation_i16n},
{"summation_i16s",(ScicosF) summation_i16s},
{"summation_i32e",(ScicosF) summation_i32e},
{"summation_i32n",(ScicosF) summation_i32n},
{"summation_i32s",(ScicosF) summation_i32s},
{"summation_i8e",(ScicosF) summation_i8e},
{"summation_i8n",(ScicosF) summation_i8n},
{"summation_i8s",(ScicosF) summation_i8s},
{"summation",(ScicosF) summation},
{"summation_ui16e",(ScicosF) summation_ui16e},
{"summation_ui16n",(ScicosF) summation_ui16n},
{"summation_ui16s",(ScicosF) summation_ui16s},
{"summation_ui32e",(ScicosF) summation_ui32e},
{"summation_ui32n",(ScicosF) summation_ui32n},
{"summation_ui32s",(ScicosF) summation_ui32s},
{"summation_ui8e",(ScicosF) summation_ui8e},
{"summation_ui8n",(ScicosF) summation_ui8n},
{"summation_ui8s",(ScicosF) summation_ui8s},
{"summation_z",(ScicosF) summation_z},
{"sum",(ScicosF) sum},
{"switch2_m",(ScicosF) switch2_m},
{"switch2",(ScicosF) switch2},
{"switchn",(ScicosF) switchn},
{"tanblk",(ScicosF) F2C(tanblk)},
{"tan_blk",(ScicosF) tan_blk},
{"tanh_blk",(ScicosF) tanh_blk},
{"tcslti4",(ScicosF) tcslti4},
{"tcslti",(ScicosF) F2C(tcslti)},
{"tcsltj4",(ScicosF) tcsltj4},
{"tcsltj",(ScicosF) F2C(tcsltj)},
{"timblk",(ScicosF) F2C(timblk)},
{"time_delay",(ScicosF) time_delay},
{"tows_c",(ScicosF) tows_c},
{"trash",(ScicosF) F2C(trash)},
{"variable_delay",(ScicosF) variable_delay},
{"writeau",(ScicosF) writeau},
{"writec",(ScicosF) writec},
{"writef",(ScicosF) F2C(writef)},
{"zcross2",(ScicosF) zcross2},
{"zcross",(ScicosF) F2C(zcross)},
{(char *) 0, (ScicosF) 0}};
 
int ntabsim= 356 ;
#endif 
/***********************************/
