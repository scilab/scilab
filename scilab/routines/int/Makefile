SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/int.a

OBJSC = tpconv.o \
	genadd.o gencopy.o genmcopy.o genrdiv.o genextrac1.o genextrac2.o \
	gendot.o genmmul.o gencheck.o geninsert1.o geninsert2.o genscal.o\
	genset.o gensubst.o genvmul.o genmtran.o genmsum.o gensum.o \
	gencusum.o genmcusum.o genmdsp.o genbitops.o mgeti.o mputi.o \
	genabs.o gendiag.o genmdiag.o gentril.o gentriu.o genkron.o \
	genmmax.o genmmin.o genmax2.o genmin2.o gifimg.o \
	genconcatrow.o genconcatcol.o


OBJSF = intelm.o intops.o  memused.o uppertype.o \
	intinttype.o convert.o bitops.o unarybit.o i_mget.o i_mput.o\
	i_a_i.o i_s_i.o i_p.o i_e.o i_i_i.o \
	i_c_i.o i_f_i.o i_m_i.o i_r_i.o i_l_i.o i_x_i.o i_d_i.o i_q_i.o \
	i_t.o i_logic.o i_sum.o i_cumsum.o i_convert.o i_abs.o i_diag.o \
	i_tril.o i_triu.o i_kron.o i_maxi.o i_mini.o  readxbm.o \
	i_matrix.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

all:: Makefile.mak

Makefile.mak	: Makefile
	$(SCIDIR)/util/Mak2VCMak Makefile

