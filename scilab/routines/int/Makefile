SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
LIBRARY = $(SCIDIR)/libs/int.a

OBJSC = tpconv.o \
	genadd.o gencopy.o genmcopy.o genrdiv.o genextrac1.o genextrac2.o \
	gendot.o genmmul.o gencheck.o geninsert1.o geninsert2.o genscal.o\
	genset.o gensubst.o genvmul.o genmtran.o genmsum.o gensum.o \
	genmprod.o genprod.o gencuprod.o genmcuprod.o \
	gencusum.o genmcusum.o genmdsp.o genbitops.o mgeti.o mputi.o \
	genabs.o gendiag.o genmdiag.o gentril.o gentriu.o genkron.o \
	genmmax.o genmmin.o genmax2.o genmin2.o gifimg.o \
	genconcatrow.o genconcatcol.o genisany.o


OBJSF = intelm.o intops.o  memused.o uppertype.o \
	intinttype.o stack_convert.o bitops.o unarybit.o i_mget.o i_mput.o\
	i_a_i.o i_s_i.o i_p.o i_e.o i_i_i.o \
	i_c_i.o i_f_i.o i_m_i.o i_r_i.o i_l_i.o i_x_i.o i_d_i.o i_q_i.o \
	i_t.o i_logic.o i_sum.o i_cumsum.o  i_prod.o i_cumprod.o \
	i_convert.o i_abs.o i_diag.o \
	i_tril.o i_triu.o i_kron.o i_maxi.o i_mini.o  readxbm.o \
	i_matrix.o

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

bitops.o: ../stack.h
stack_convert.o: ../stack.h
creimat.o: ../stack.h
getimat.o: ../stack.h
i_a_i.o: ../stack.h
i_abs.o: ../stack.h
i_c_i.o: ../stack.h
i_convert.o: ../stack.h
i_cumsum.o: ../stack.h
i_d_i.o: ../stack.h
i_diag.o: ../stack.h
i_e.o: ../stack.h
i_f_i.o: ../stack.h
i_i_i.o: ../stack.h
i_kron.o: ../stack.h
i_l_i.o: ../stack.h
i_logic.o: ../stack.h
i_m_i.o: ../stack.h
i_matrix.o: ../stack.h
i_maxi.o: ../stack.h
i_mget.o: ../stack.h
i_mini.o: ../stack.h
i_mput.o: ../stack.h
i_p.o: ../stack.h
i_q_i.o: ../stack.h
i_r_i.o: ../stack.h
i_s_i.o: ../stack.h
i_sum.o: ../stack.h
i_t.o: ../stack.h
i_tril.o: ../stack.h
i_triu.o: ../stack.h
i_x_i.o: ../stack.h
intelm.o: ../stack.h
intinttype.o: ../stack.h
intops.o: ../stack.h
iread.o: ../stack.h
readgif.o: ../stack.h
readxbm.o: ../stack.h
unarybit.o: ../stack.h

db2int.o: db2int.c ../machine.h
genabs.o: genabs.c ../machine.h
genadd.o: genadd.c ../machine.h
genbitops.o: genbitops.c ../machine.h
genbitor.o: genbitor.c ../machine.h
gencheck.o: gencheck.c ../machine.h
genconcatcol.o: genconcatcol.c ../machine.h
genconcatrow.o: genconcatrow.c ../machine.h
gencopy.o: gencopy.c ../machine.h
gencusum.o: gencusum.c ../machine.h
gendiag.o: gendiag.c ../machine.h
gendot.o: gendot.c ../machine.h
genextrac1.o: genextrac1.c ../machine.h
genextrac2.o: genextrac2.c ../machine.h
geninsert1.o: geninsert1.c ../machine.h
geninsert2.o: geninsert2.c ../machine.h
genkron.o: genkron.c ../machine.h
genmax2.o: genmax2.c ../machine.h
genmcopy.o: genmcopy.c ../machine.h
genmcusum.o: genmcusum.c ../machine.h
genmdiag.o: genmdiag.c ../machine.h
genmdsp.o: genmdsp.c ../machine.h
genmin2.o: genmin2.c ../machine.h
genmmax.o: genmmax.c ../machine.h
genmmin.o: genmmin.c ../machine.h
genmmul.o: genmmul.c ../machine.h
genmsum.o: genmsum.c ../machine.h
genmtran.o: genmtran.c ../machine.h
genrdiv.o: genrdiv.c ../machine.h
genrowconc.o: genrowconc.c ../machine.h
genscal.o: genscal.c ../machine.h
genset.o: genset.c ../machine.h
gensubst.o: gensubst.c ../machine.h
gensum.o: gensum.c ../machine.h
gentril.o: gentril.c ../machine.h
gentriu.o: gentriu.c ../machine.h
genvmul.o: genvmul.c ../machine.h
gifimg.o: gifimg.c ../gd/gd.h ../machine.h
mgeti.o: mgeti.c ../machine.h ../fileio/st.h ../fileio/wav.h
mputi.o: mputi.c ../machine.h ../fileio/st.h ../fileio/wav.h
readgifimg.o: readgifimg.c ../gd/gd.h ../machine.h
tpconv.o: tpconv.c ../machine.h

