SHELL = /bin/sh
SCIDIR=../../..

LIBRARY = $(SCIDIR)/libs/libf2c.a

# If your system lacks onexit() and you are not using an
# ANSI C compiler, then you should add -DNO_ONEXIT to CFLAGS,
# e.g., by changing the above "CFLAGS =" line to
# CFLAGS = -O -DNO_ONEXIT
# On at least some Sun systems, it is more appropriate to change the
# "CFLAGS =" line to
# CFLAGS = -O -Donexit=on_exit

OBJI =	backspace.o close.o dfe.o dolio.o due.o endfile.o err.o \
	fmt.o fmtlib.o ftell_.o iio.o ilnw.o inquire.o lread.o lwrite.o \
	open.o rdfmt.o rewind.o rsfe.o rsli.o rsne.o sfe.o sue.o typesize.o \
	uio.o util.o wref.o wrtfmt.o wsfe.o wsle.o wsne.o xwsne.o

MAIN=main.o

ENT=entries.o

MISC =	F77_aloc.o Version.o  s_rnge.o abort_.o getarg_.o iargc_.o \
	getenv_.o signal_.o s_stop.o s_paus.o system_.o cabs.o\
	derf_.o derfc_.o erf_.o  sig_die.o exit.o

# not on windows 

MISC1 = erfc_.o

POW =	pow_ci.o pow_dd.o pow_di.o pow_hh.o pow_ii.o  pow_ri.o pow_zi.o pow_zz.o
CX =	c_abs.o c_cos.o c_div.o c_exp.o c_log.o c_sin.o c_sqrt.o
DCX =	z_abs.o z_cos.o z_div.o z_exp.o z_log.o z_sin.o z_sqrt.o
REAL =	r_abs.o r_acos.o r_asin.o r_atan.o r_atn2.o r_cnjg.o r_cos.o\
	r_cosh.o r_dim.o r_exp.o r_imag.o r_int.o\
	r_lg10.o r_log.o r_mod.o r_nint.o r_sign.o\
	r_sin.o r_sinh.o r_sqrt.o r_tan.o r_tanh.o
DBL =	d_abs.o d_acos.o d_asin.o d_atan.o d_atn2.o\
	d_cnjg.o d_cos.o d_cosh.o d_dim.o d_exp.o\
	d_imag.o d_int.o d_lg10.o d_log.o d_mod.o\
	d_nint.o d_prod.o d_sign.o d_sin.o d_sinh.o\
	d_sqrt.o d_tan.o d_tanh.o
INT =	i_abs.o i_dim.o i_dnnt.o i_indx.o i_len.o i_mod.o i_nint.o i_sign.o
HALF =	h_abs.o h_dim.o h_dnnt.o h_indx.o h_len.o h_mod.o  h_nint.o h_sign.o
CMP =	l_ge.o l_gt.o l_le.o l_lt.o hl_ge.o hl_gt.o hl_le.o hl_lt.o
EFL =	ef1asc_.o ef1cmc_.o
CHAR =	s_cat.o s_cmp.o s_copy.o
F90BIT = lbitbits.o lbitshft.o

OBJSC = $(ENT) $(OBJI) $(MISC) $(POW) $(CX) $(DCX) $(REAL) $(DBL) $(INT) \
		$(HALF) $(CMP) $(EFL) $(CHAR) $(F90BIT)

OBJSF = 

all:: main.o ../../../libs/main.o 

../../../libs/main.o : main.o 
	cp main.o ../../../libs/main.o

include ../../../Makefile.incl

CFLAGS = -mno-cygwin $(CC_OPTIONS) -D_POSIX_SOURCE  -DSkip_f2c_Undefs -DMSDOS

include ../../Make.lib



