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
	genmprod.obj genprod.obj gencuprod.obj genmcuprod.obj \
	gencusum.obj genmcusum.obj genmdsp.obj genbitops.obj mgeti.obj mputi.obj \
	genabs.obj gendiag.obj genmdiag.obj gentril.obj gentriu.obj genkron.obj \
	genmmax.obj genmmin.obj genmax2.obj genmin2.obj gifimg.obj \
	genconcatrow.obj genconcatcol.obj genisany.obj


OBJSF = intelm.obj intops.obj  memused.obj uppertype.obj \
	intinttype.obj stack_convert.obj bitops.obj unarybit.obj i_mget.obj i_mput.obj\
	i_a_i.obj i_s_i.obj i_p.obj i_e.obj i_i_i.obj \
	i_c_i.obj i_f_i.obj i_m_i.obj i_r_i.obj i_l_i.obj i_x_i.obj i_d_i.obj i_q_i.obj \
	i_t.obj i_logic.obj i_sum.obj i_cumsum.obj  i_prod.obj i_cumprod.obj \
	i_convert.obj i_abs.obj i_diag.obj \
	i_tril.obj i_triu.obj i_kron.obj i_maxi.obj i_mini.obj  readxbm.obj \
	i_matrix.obj

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib.mak

bitops.obj: ../stack.h
stack_convert.obj: ../stack.h
creimat.obj: ../stack.h
getimat.obj: ../stack.h
i_a_i.obj: ../stack.h
i_abs.obj: ../stack.h
i_c_i.obj: ../stack.h
i_convert.obj: ../stack.h
i_cumsum.obj: ../stack.h
i_d_i.obj: ../stack.h
i_diag.obj: ../stack.h
i_e.obj: ../stack.h
i_f_i.obj: ../stack.h
i_i_i.obj: ../stack.h
i_kron.obj: ../stack.h
i_l_i.obj: ../stack.h
i_logic.obj: ../stack.h
i_m_i.obj: ../stack.h
i_matrix.obj: ../stack.h
i_maxi.obj: ../stack.h
i_mget.obj: ../stack.h
i_mini.obj: ../stack.h
i_mput.obj: ../stack.h
i_p.obj: ../stack.h
i_q_i.obj: ../stack.h
i_r_i.obj: ../stack.h
i_s_i.obj: ../stack.h
i_sum.obj: ../stack.h
i_t.obj: ../stack.h
i_tril.obj: ../stack.h
i_triu.obj: ../stack.h
i_x_i.obj: ../stack.h
intelm.obj: ../stack.h
intinttype.obj: ../stack.h
intops.obj: ../stack.h
iread.obj: ../stack.h
readgif.obj: ../stack.h
readxbm.obj: ../stack.h
unarybit.obj: ../stack.h

db2int.obj: db2int.c ../machine.h
genabs.obj: genabs.c ../machine.h
genadd.obj: genadd.c ../machine.h
genbitops.obj: genbitops.c ../machine.h
genbitor.obj: genbitor.c ../machine.h
gencheck.obj: gencheck.c ../machine.h
genconcatcol.obj: genconcatcol.c ../machine.h
genconcatrow.obj: genconcatrow.c ../machine.h
gencopy.obj: gencopy.c ../machine.h
gencusum.obj: gencusum.c ../machine.h
gendiag.obj: gendiag.c ../machine.h
gendot.obj: gendot.c ../machine.h
genextrac1.obj: genextrac1.c ../machine.h
genextrac2.obj: genextrac2.c ../machine.h
geninsert1.obj: geninsert1.c ../machine.h
geninsert2.obj: geninsert2.c ../machine.h
genkron.obj: genkron.c ../machine.h
genmax2.obj: genmax2.c ../machine.h
genmcopy.obj: genmcopy.c ../machine.h
genmcusum.obj: genmcusum.c ../machine.h
genmdiag.obj: genmdiag.c ../machine.h
genmdsp.obj: genmdsp.c ../machine.h
genmin2.obj: genmin2.c ../machine.h
genmmax.obj: genmmax.c ../machine.h
genmmin.obj: genmmin.c ../machine.h
genmmul.obj: genmmul.c ../machine.h
genmsum.obj: genmsum.c ../machine.h
genmtran.obj: genmtran.c ../machine.h
genrdiv.obj: genrdiv.c ../machine.h
genrowconc.obj: genrowconc.c ../machine.h
genscal.obj: genscal.c ../machine.h
genset.obj: genset.c ../machine.h
gensubst.obj: gensubst.c ../machine.h
gensum.obj: gensum.c ../machine.h
gentril.obj: gentril.c ../machine.h
gentriu.obj: gentriu.c ../machine.h
genvmul.obj: genvmul.c ../machine.h
gifimg.obj: gifimg.c ../gd/gd.h ../machine.h
mgeti.obj: mgeti.c ../machine.h ../fileio/st.h ../fileio/wav.h
mputi.obj: mputi.c ../machine.h ../fileio/st.h ../fileio/wav.h
readgifimg.obj: readgifimg.c ../gd/gd.h ../machine.h
tpconv.obj: tpconv.c ../machine.h

