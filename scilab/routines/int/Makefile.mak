#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/int.lib

OBJSC = tpconv.obj \
	genadd.obj gencopy.obj genmcopy.obj genrdiv.obj genextrac1.obj genextrac2.obj \
	gendot.obj genmmul.obj gencheck.obj geninsert1.obj geninsert2.obj genscal.obj\
	genset.obj gensubst.obj genvmul.obj genmtran.obj genmsum.obj gensum.obj \
	gencusum.obj genmcusum.obj genmdsp.obj genbitops.obj mgeti.obj mputi.obj \
	genabs.obj gendiag.obj genmdiag.obj gentril.obj gentriu.obj genkron.obj \
	genmmax.obj genmmin.obj genmax2.obj genmin2.obj gifimg.obj \
	genconcatrow.obj genconcatcol.obj


OBJSF = intelm.obj intops.obj  memused.obj uppertype.obj \
	intinttype.obj convert.obj bitops.obj unarybit.obj i_mget.obj i_mput.obj\
	i_a_i.obj i_s_i.obj i_p.obj i_e.obj i_i_i.obj \
	i_c_i.obj i_f_i.obj i_m_i.obj i_r_i.obj i_l_i.obj i_x_i.obj i_d_i.obj i_q_i.obj \
	i_t.obj i_logic.obj i_sum.obj i_cumsum.obj i_convert.obj i_abs.obj i_diag.obj \
	i_tril.obj i_triu.obj i_kron.obj i_maxi.obj i_mini.obj  readxbm.obj \
	i_matrix.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak



Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

