SHELL = /bin/sh
SCIDIR=../../..
SCIDIR1=..\..\..

LIBRARY = $(SCIDIR)/libs/libf2c.lib
DLL = $(SCIDIR)/libs/libf2c.dll

MAIN = main.obj 

OBJI =	backspace.obj close.obj dfe.obj dolio.obj due.obj \
	endfile.obj err.obj fmt.obj fmtlib.obj ftell_.obj iio.obj ilnw.obj \
	inquire.obj lread.obj lwrite.obj open.obj rdfmt.obj rewind.obj \
	rsfe.obj rsli.obj rsne.obj sfe.obj sue.obj typesize.obj \
	uio.obj util.obj wref.obj wrtfmt.obj wsfe.obj wsle.obj wsne.obj \
	xwsne.obj

MISC = F77_aloc.obj Version.obj s_rnge.obj abort_.obj getarg_.obj iargc_.obj \
	getenv_.obj signal_.obj s_stop.obj s_paus.obj system_.obj cabs.obj\
	derf_.obj derfc_.obj erf_.obj erfc_.obj sig_die.obj exit.obj
POW = pow_ci.obj pow_dd.obj pow_di.obj pow_hh.obj pow_ii.obj  pow_ri.obj \
	pow_zi.obj pow_zz.obj
CX = c_abs.obj c_cos.obj c_div.obj c_exp.obj c_log.obj c_sin.obj c_sqrt.obj
DCX = z_abs.obj z_cos.obj z_div.obj z_exp.obj z_log.obj z_sin.obj z_sqrt.obj
REAL = r_abs.obj r_acos.obj r_asin.obj r_atan.obj r_atn2.obj r_cnjg.obj r_cos.obj\
	r_cosh.obj r_dim.obj r_exp.obj r_imag.obj r_int.obj\
	r_lg10.obj r_log.obj r_mod.obj r_nint.obj r_sign.obj\
	r_sin.obj r_sinh.obj r_sqrt.obj r_tan.obj r_tanh.obj
DBL = d_abs.obj d_acos.obj d_asin.obj d_atan.obj d_atn2.obj\
	d_cnjg.obj d_cos.obj d_cosh.obj d_dim.obj d_exp.obj\
	d_imag.obj d_int.obj d_lg10.obj d_log.obj d_mod.obj\
	d_nint.obj d_prod.obj d_sign.obj d_sin.obj d_sinh.obj\
	d_sqrt.obj d_tan.obj d_tanh.obj
INT = i_abs.obj i_dim.obj i_dnnt.obj i_indx.obj i_len.obj i_mod.obj i_nint.obj i_sign.obj
HALF = h_abs.obj h_dim.obj h_dnnt.obj h_indx.obj h_len.obj h_mod.obj  h_nint.obj h_sign.obj
CMP = l_ge.obj l_gt.obj l_le.obj l_lt.obj hl_ge.obj hl_gt.obj hl_le.obj hl_lt.obj
EFL = ef1asc_.obj ef1cmc_.obj
CHAR = s_cat.obj s_cmp.obj s_copy.obj
F90BIT = lbitbits.obj lbitshft.obj

OBJSC = $(OBJI) $(MISC) $(POW) $(CX) $(DCX) $(REAL) $(DBL) $(INT) \
		$(HALF) $(CMP) $(EFL) $(CHAR) $(F90BIT)

OBJSF = 

all:: main.obj 

include ../../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) -DSkip_f2c_Undefs -D_POSIX_SOURCE -DMSDOS

include ../../Make.lib.mak

Version.obj: Version.c
	$(CC) $(CC_OPTIONS) -c Version.c

backspace.obj: fio.h
close.obj: fio.h
dfe.obj: fio.h
dfe.obj: fmt.h
due.obj: fio.h
endfile.obj: fio.h rawio.h
err.obj: fio.h rawio.h
fmt.obj: fio.h
fmt.obj: fmt.h
ftell_.obj: fio.h
iio.obj: fio.h
iio.obj: fmt.h
ilnw.obj: fio.h
ilnw.obj: lio.h
inquire.obj: fio.h
lread.obj: fio.h
lread.obj: fmt.h
lread.obj: lio.h
lread.obj: fp.h
lwrite.obj: fio.h
lwrite.obj: fmt.h
lwrite.obj: lio.h
open.obj: fio.h rawio.h
rdfmt.obj: fio.h
rdfmt.obj: fmt.h
rdfmt.obj: fp.h
rewind.obj: fio.h
rsfe.obj: fio.h
rsfe.obj: fmt.h
rsli.obj: fio.h
rsli.obj: lio.h
rsne.obj: fio.h
rsne.obj: lio.h
sfe.obj: fio.h
sue.obj: fio.h
uio.obj: fio.h
util.obj: fio.h
wref.obj: fio.h
wref.obj: fmt.h
wref.obj: fp.h
wrtfmt.obj: fio.h
wrtfmt.obj: fmt.h
wsfe.obj: fio.h
wsfe.obj: fmt.h
wsle.obj: fio.h
wsle.obj: fmt.h
wsle.obj: lio.h
wsne.obj: fio.h
wsne.obj: lio.h
xwsne.obj: fio.h
xwsne.obj: lio.h
xwsne.obj: fmt.h

.c.txt	:
	$(CC) $(CFLAGS) -E $*.c 



