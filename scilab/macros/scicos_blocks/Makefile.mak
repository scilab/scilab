#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = blockslib
NAM = SCI/macros/scicos_blocks

MACROS=SPLIT_f.sci  GAIN_f.sci IN_f.sci CLKIN_f.sci OUT_f.sci CLKOUT_f.sci \
	CLKOUTV_f.sci  CLKINV_f.sci CLKSOMV_f.sci  GAINBLK_f.sci \
	QUANT_f.sci CLKSPLIT_f.sci CLKSOM_f.sci EVTDLY_f.sci\
	SAT_f.sci SOM_f.sci SUPER_f.sci SCOPE_f.sci CLSS_f.sci DLSS_f.sci\
	CLR_f.sci DLR_f.sci ZCROSS_f.sci m_sin.sci TRASH_f.sci \
	CONST_f.sci DELAY_f.sci SELECT_f.sci PROD_f.sci TEXT_f.sci CLOCK_f.sci\
	EVTGEN_f.sci IFTHEL_f.sci func_block.sci scifunc_block.sci\
	EXPBLK_f.sci WFILE_f.sci SINBLK_f.sci COSBLK_f.sci TANBLK_f.sci\
	INVBLK_f.sci LOGBLK_f.sci POWBLK_f.sci HALT_f.sci GENSIN_f.sci \
	RAND_f.sci ABSBLK_f.sci LOOKUP_f.sci TIME_f.sci NEGTOPOS_f.sci\
	POSTONEG_f.sci GENERAL_f.sci REGISTER_f.sci CLINDUMMY_f.sci\
	GENSQR_f.sci MFCLCK_f.sci SAWTOOTH_f.sci MCLOCK_f.sci TCLSS_f.sci\
	SCOPXY_f.sci EVENTSCOPE_f.sci INTEGRAL_f.sci RFILE_f.sci ANIMXY_f.sci\
	CURV_f.sci MIN_f.sci MAX_f.sci DLRADAPT_f.sci ANDLOG_f.sci\
	MUX_f.sci DEMUX_f.sci DELAYV_f.sci SAMPLEHOLD_f.sci ANDBLK.sci \
	DOLLAR_f.sci AFFICH_f.sci MSCOPE_f.sci NRMSOM_f.sci ISELECT_f.sci \
	ESELECT_f.sci BIGSOM_f.sci INTRPLBLK_f.sci INTRP2BLK_f.sci SWITCH_f.sci \
	RELAY_f.sci FSV_f.sci MEMORY_f.sci FSCOPE_f.sci READC_f.sci WRITEC_f.sci \
	WRITEAU_f.sci READAU_f.sci SUM_f.sci

include ../Make.lib.mak
