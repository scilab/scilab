##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

INTEGER_C_SOURCES = src/c/genconcatcol.c \
src/c/genprod.c \
src/c/genrdiv.c \
src/c/genmin2.c \
src/c/i_nextj.c \
src/c/genmax2.c \
src/c/genextrac1.c \
src/c/genextrac2.c \
src/c/gencopy.c \
src/c/gentril.c \
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
src/fortran/stack_convert.f \
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

libsciinteger_la_CFLAGS=  	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes/

pkglib_LTLIBRARIES = libsciinteger.la

libsciinteger_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libsciinteger_la_SOURCES = $(INTEGER_C_SOURCES) $(INTEGER_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC=  $(INTEGER_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciinteger_la_CFLAGS)

# cyclic $(top_builddir)/modules/polynomials/libscipolynomials.la
libsciinteger_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la  $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/fileio/libscifileio.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/libs/libst/libscilibst.la



#### Target ######
modulename=integer


#### integer : Conf files ####
libsciinteger_la_rootdir = $(mydatadir)
libsciinteger_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### integer : init scripts ####
libsciinteger_la_etcdir = $(mydatadir)/etc
libsciinteger_la_etc_DATA = etc/integer.quit etc/integer.start

#### integer : gateway declaration ####
libsciinteger_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciinteger_la_sci_gateway_DATA = sci_gateway/integer_gateway.xml

include $(top_srcdir)/Makefile.incl.am
