##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

INTEGER_C_SOURCES = src/c/genconcatcol.c \
src/c/genprod.c \
src/c/genrdiv.c \
src/c/mputi.c \
src/c/genmin2.c \
src/c/i_nextj.c \
src/c/genmax2.c \
src/c/genextrac1.c \
src/c/genextrac2.c \
src/c/gencopy.c \
src/c/gentril.c \
src/c/mgeti.c \
src/c/gencheck.c \
src/c/tpconv.c \
src/c/genmdiag.c \
src/c/genscal.c \
src/c/gentriu.c \
src/c/genvmul.c \
src/c/genconcatrow.c \
src/c/geninsert1.c \
src/c/geninsert2.c \
src/c/gendot.c \
src/c/db2int.c \
src/c/genadd.c \
src/c/genset.c \
src/c/gencusum.c \
src/c/genmmin.c \
src/c/gensubst.c \
src/c/genmmax.c \
src/c/genmdsp.c \
src/c/gendiag.c \
src/c/gensum.c \
src/c/genabs.c \
src/c/genmcusum.c \
src/c/genmcuprod.c \
src/c/genkron.c \
src/c/genmmul.c \
src/c/genmsum.c \
src/c/genmprod.c \
src/c/genmtran.c \
src/c/genisany.c \
src/c/genmcopy.c \
src/c/gencuprod.c \
src/c/genbitops.c \
src/c/genimpl.c

INTEGER_FORTRAN_SOURCES = src/fortran/unarybit.f \
src/fortran/i_mput.f \
src/fortran/i_logic.f \
src/fortran/i_a_i.f \
src/fortran/i_b_i.f \
src/fortran/i_c_i.f \
src/fortran/i_d_i.f \
src/fortran/i_mini.f \
src/fortran/intinttype.f \
src/fortran/i_f_i.f \
src/fortran/i_maxi.f \
src/fortran/i_matrix.f \
src/fortran/i_i_i.f \
src/fortran/i_l_i.f \
src/fortran/i_m_i.f \
src/fortran/intops.f \
src/fortran/i_prod.f \
src/fortran/i_convert.f \
src/fortran/i_q_i.f \
src/fortran/i_r_i.f \
src/fortran/i_tril.f \
src/fortran/i_s_i.f \
src/fortran/i_x_i.f \
src/fortran/uppertype.f \
src/fortran/i_triu.f \
src/fortran/i_e.f \
src/fortran/memused.f \
src/fortran/bitops.f \
src/fortran/i_mget.f \
src/fortran/i_p.f \
src/fortran/i_t.f \
src/fortran/i_sum.f \
src/fortran/i_diag.f \
src/fortran/i_abs.f \
src/fortran/convert.f \
src/fortran/i_cumprod.f \
src/fortran/i_cumsum.f \
src/fortran/i_kron.f

GATEWAY_C_SOURCES = sci_gateway/c/gw_integer.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_i_matrix.f \
sci_gateway/fortran/sci_i_mput.f \
sci_gateway/fortran/sci_i_max.f \
sci_gateway/fortran/sci_int32.f \
sci_gateway/fortran/sci_int16.f \
sci_gateway/fortran/sci_i_mini.f \
sci_gateway/fortran/sci_mgeti.f \
sci_gateway/fortran/sci_i_maxi.f \
sci_gateway/fortran/sci_i_p.f \
sci_gateway/fortran/sci_uint8.f \
sci_gateway/fortran/sci_double.f \
sci_gateway/fortran/sci_i_sum.f \
sci_gateway/fortran/sci_i_prod.f \
sci_gateway/fortran/sci_i_cumsum.f \
sci_gateway/fortran/sci_i_diag.f \
sci_gateway/fortran/sci_i_abs.f \
sci_gateway/fortran/sci_i_cumprod.f \
sci_gateway/fortran/sci_i_tril.f \
sci_gateway/fortran/sci_int8.f \
sci_gateway/fortran/sci_uint32.f \
sci_gateway/fortran/sci_uint16.f \
sci_gateway/fortran/sci_iconvert.f \
sci_gateway/fortran/sci_i_triu.f \
sci_gateway/fortran/sci_i_min.f \
sci_gateway/fortran/sci_inttype.f

libsciinteger_la_CFLAGS=  -Iincludes/ 

pkglib_LTLIBRARIES = libsciinteger.la

libsciinteger_la_SOURCES = $(INTEGER_C_SOURCES) $(INTEGER_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsciinteger_la_LIBADD = \
	$(top_builddir)/modules/polynomials/libscipolynomials.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/fileio/libscifileio.la 




#### Target ######
target=$(datadir)/integer


#### integer : Conf files ####
libsciinteger_la_rootdir = $(target)
libsciinteger_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### integer : Scilab macros ###
libsciinteger_la_macrodir = $(target)/macros
MACROS = macros/%i_plot2d1.sci \
macros/%i_mfprintf.sci \
macros/%i_plot2d2.sci \
macros/%i_contour.sci \
macros/%i_sign.sci \
macros/%i_j_i.sci \
macros/%i_msprintf.sci \
macros/%i_a_s.sci \
macros/%s_a_i.sci \
macros/%i_plot2d.sci \
macros/%i_d_s.sci \
macros/%s_d_i.sci \
macros/%i_string.sci \
macros/%i_p_i.sci \
macros/%i_champ1.sci \
macros/%s_i_i.sci \
macros/%i_j_s.sci \
macros/%s_j_i.sci \
macros/%i_l_s.sci \
macros/%s_l_i.sci \
macros/%i_m_s.sci \
macros/%s_m_i.sci \
macros/%i_n_s.sci \
macros/%i_mprintf.sci \
macros/%i_contour2d.sci \
macros/%s_n_i.sci \
macros/%i_o_s.sci \
macros/%i_Matplot.sci \
macros/%s_o_i.sci \
macros/%i_p_s.sci \
macros/%s_p_i.sci \
macros/%i_q_s.sci \
macros/%i_1_s.sci \
macros/%s_1_i.sci \
macros/%s_q_i.sci \
macros/%i_r_s.sci \
macros/%i_2_s.sci \
macros/%s_2_i.sci \
macros/%s_r_i.sci \
macros/%i_s_s.sci \
macros/%i_3_s.sci \
macros/%s_3_i.sci \
macros/%s_s_i.sci \
macros/%i_4_s.sci \
macros/%s_4_i.sci \
macros/%i_champ.sci \
macros/%i_x_s.sci \
macros/%s_x_i.sci \
macros/%i_fft.sci macros/%i_1_s.bin \
macros/%i_2_s.bin \
macros/%i_3_s.bin \
macros/%i_4_s.bin \
macros/%i_a_s.bin \
macros/%i_champ1.bin \
macros/%i_champ.bin \
macros/%i_contour2d.bin \
macros/%i_contour.bin \
macros/%i_d_s.bin \
macros/%i_fft.bin \
macros/%i_j_i.bin \
macros/%i_j_s.bin \
macros/%i_l_s.bin \
macros/%i_Matplot.bin \
macros/%i_mfprintf.bin \
macros/%i_mprintf.bin \
macros/%i_msprintf.bin \
macros/%i_m_s.bin \
macros/%i_n_s.bin \
macros/%i_o_s.bin \
macros/%i_p_i.bin \
macros/%i_plot2d1.bin \
macros/%i_plot2d2.bin \
macros/%i_plot2d.bin \
macros/%i_p_s.bin \
macros/%i_q_s.bin \
macros/%i_r_s.bin \
macros/%i_sign.bin \
macros/%i_s_s.bin \
macros/%i_string.bin \
macros/%i_x_s.bin \
macros/%s_1_i.bin \
macros/%s_2_i.bin \
macros/%s_3_i.bin \
macros/%s_4_i.bin \
macros/%s_a_i.bin \
macros/%s_d_i.bin \
macros/%s_i_i.bin \
macros/%s_j_i.bin \
macros/%s_l_i.bin \
macros/%s_m_i.bin \
macros/%s_n_i.bin \
macros/%s_o_i.bin \
macros/%s_p_i.bin \
macros/%s_q_i.bin \
macros/%s_r_i.bin \
macros/%s_s_i.bin \
macros/%s_x_i.bin macros/names macros/lib macros/buildmacros.sce


#### integer : init scripts #### 
libsciinteger_la_etcdir = $(target)/etc
libsciinteger_la_etc_DATA = etc/integer.quit etc/integer.start

#### integer : gateway declaration #### 
libsciinteger_la_sci_gatewaydir = $(target)/sci_gateway
libsciinteger_la_sci_gateway_DATA = sci_gateway/integer

#### integer : include files ####
libsciinteger_la_includedir=$(sciincludedir)/integer
libsciinteger_la_include_HEADERS = includes/gw_integer.h \
includes/def.h

include $(top_srcdir)/Makefile.incl.am
