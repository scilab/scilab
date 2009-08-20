##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

if PVM

PVM_C_SOURCES = src/c/pvm_proc_ctrl.c \
src/c/sci_tools.c \
src/c/pvm_recv.c \
src/c/scipvmf77.c \
src/c/varpack.c \
src/c/pvm_grp.c \
src/c/pvm_send.c \
src/c/pvm_info.c \
src/c/with_pvm.c

GATEWAY_C_SOURCES = sci_gateway/c/sci_pvm_get_timer.c \
sci_gateway/c/sci_pvm_gettid.c \
sci_gateway/c/sci_pvm_gsize.c \
sci_gateway/c/sci_pvm_sci2f77.c \
sci_gateway/c/sci_pvm_f772sci.c \
sci_gateway/c/sci_pvm_delhosts.c \
sci_gateway/c/sci_pvm_spawn.c \
sci_gateway/c/sci_pvm_send.c \
sci_gateway/c/sci_pvm_parent.c \
sci_gateway/c/sci_pvm_error.c \
sci_gateway/c/sci_pvm_set_timer.c \
sci_gateway/c/sci_pvm_kill.c \
sci_gateway/c/sci_pvm_bcast.c \
sci_gateway/c/sci_pvm_recv.c \
sci_gateway/c/sci_pvm_reduce.c \
sci_gateway/c/gw_pvm.c \
sci_gateway/c/sci_pvm_spawn_independent.c \
sci_gateway/c/sci_pvm_probe.c \
sci_gateway/c/sci_pvm_addhosts.c \
sci_gateway/c/sci_pvm_joingroup.c \
sci_gateway/c/sci_pvm_getinst.c \
sci_gateway/c/sci_pvm_tidtohost.c \
sci_gateway/c/sci_pvm_tasks.c \
sci_gateway/c/sci_pvm_mytid.c \
sci_gateway/c/sci_pvm_barrier.c \
sci_gateway/c/sci_pvm_halt.c \
sci_gateway/c/sci_pvm_bufinfo.c \
sci_gateway/c/sci_pvm_error_mode.c \
sci_gateway/c/sci_pvm_start.c \
sci_gateway/c/sci_pvm_lvgroup.c \
sci_gateway/c/sci_pvm_send_var.c \
sci_gateway/c/sci_pvm_config.c \
sci_gateway/c/sci_pvm_recv_var.c \
sci_gateway/c/sci_pvm_exit.c

libscipvm_la_CFLAGS=  -I$(srcdir)/includes/ \
	-I$(top_srcdir)/libs/MALLOC/includes/ -I$(srcdir)/src/c/  \
	-I$(top_srcdir)/modules/output_stream/includes/ \
	-I$(top_srcdir)/modules/io/includes/ \
	-I$(top_srcdir)/modules/fileio/includes/ \
	-I$(top_srcdir)/modules/api_scilab/includes \
	-I$(top_srcdir)/modules/operations/includes/
	$(PVM_INCLUDE)

pkglib_LTLIBRARIES = libscipvm.la

libscipvm_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(PVM_LIB)

libscipvm_la_SOURCES = $(PVM_C_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= $(PVM_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscipvm_la_CFLAGS)

libscipvm_la_LIBADD =  $(top_builddir)/modules/output_stream/libscioutput_stream.la

modulename=pvm

#### pvm : Conf files ####
libscipvm_la_rootdir = $(mydatadir)
libscipvm_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### pvm : init scripts ####
libscipvm_la_etcdir = $(mydatadir)/etc
libscipvm_la_etc_DATA = etc/pvm.quit etc/pvm.start

#### pvm : gateway declaration ####
libscipvm_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscipvm_la_sci_gateway_DATA = sci_gateway/pvm_gateway.xml

#### pvm : include files ####
libscipvm_la_includedir=$(pkgincludedir)/pvm
libscipvm_la_include_HEADERS = includes/gw_pvm.h \
includes/with_pvm.h


include $(top_srcdir)/Makefile.incl.am

else

PVM_C_SOURCES = src/nopvm/nopvm.c

pkglib_LTLIBRARIES = libscipvm.la

libscipvm_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscipvm_la_SOURCES = $(PVM_C_SOURCES)

libscipvm_la_LIBADD = \
	$(top_builddir)/modules/core/libscicore.la

libscipvm_la_CFLAGS = \
						-I$(top_srcdir)/modules/api_scilab/includes \
						-I$(top_srcdir)/modules/output_stream/includes/


modulename=pvm

#### pvm : init scripts ####
libscipvm_la_etcdir = $(mydatadir)/etc
libscipvm_la_etc_DATA = etc/pvm.quit etc/pvm.start

include $(top_srcdir)/Makefile.incl.am

endif
