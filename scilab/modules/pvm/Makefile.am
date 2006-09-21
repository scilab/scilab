##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

if PVM
PVM_C_SOURCES = src/pvm_proc_ctrl.c \
src/sci_tools.c \
src/pvm_recv.c \
src/scipvmf77.c \
src/varpack.c \
src/pvm_grp.c \
src/pvm_send.c \
src/pvm_info.c

GATEWAY_C_SOURCES = sci_gateway/sci_pvm_get_timer.c \
sci_gateway/sci_pvm_gettid.c \
sci_gateway/sci_pvm_gsize.c \
sci_gateway/sci_pvm_sci2f77.c \
sci_gateway/sci_pvm_f772sci.c \
sci_gateway/sci_pvm_delhosts.c \
sci_gateway/sci_pvm_spawn.c \
sci_gateway/sci_pvm_send.c \
sci_gateway/sci_pvm_parent.c \
sci_gateway/sci_pvm_error.c \
sci_gateway/sci_pvm_set_timer.c \
sci_gateway/sci_pvm_kill.c \
sci_gateway/sci_pvm_bcast.c \
sci_gateway/sci_pvm_recv.c \
sci_gateway/sci_pvm_reduce.c \
sci_gateway/gw_pvm.c \
sci_gateway/sci_pvm_spawn_independent.c \
sci_gateway/sci_pvm_probe.c \
sci_gateway/sci_pvm_addhosts.c \
sci_gateway/sci_pvm_joingroup.c \
sci_gateway/sci_pvm_getinst.c \
sci_gateway/sci_pvm_tidtohost.c \
sci_gateway/sci_pvm_tasks.c \
sci_gateway/sci_pvm_mytid.c \
sci_gateway/sci_pvm_barrier.c \
sci_gateway/sci_pvm_halt.c \
sci_gateway/sci_pvm_bufinfo.c \
sci_gateway/sci_pvm_error_mode.c \
sci_gateway/sci_pvm_start.c \
sci_gateway/sci_pvm_lvgroup.c \
sci_gateway/sci_pvm_send_var.c \
sci_gateway/sci_pvm_config.c \
sci_gateway/sci_pvm_recv_var.c \
sci_gateway/sci_pvm_exit.c

libscipvm_la_includedir=$(includedir)/pvm

libscipvm_la_include_HEADERS = includes/sci_pvm_get_timer.h \
includes/sci_pvm_exit.h \
includes/sci_pvm_sci2f77.h \
includes/sci_pvm_gettid.h \
includes/sci_pvm_gsize.h \
includes/sci_pvm_f772sci.h \
includes/sci_pvm_delhosts.h \
includes/sci_pvm_spawn.h \
includes/sci_pvm_send.h \
includes/sci_pvm_parent.h \
includes/sci_pvm_error.h \
includes/sci_pvm_set_timer.h \
includes/sci_pvm_kill.h \
includes/sci_pvm_bcast.h \
includes/sci_pvm_recv.h \
includes/sci_pvm_reduce.h \
includes/gw_pvm.h \
includes/sci_pvm_spawn_independent.h \
includes/sci_pvm_probe.h \
includes/sci_pvm_addhosts.h \
includes/sci_pvm_joingroup.h \
includes/sci_pvm_getinst.h \
includes/sci_pvm_tidtohost.h \
includes/sci_pvm_tasks.h \
includes/sci_pvm_mytid.h \
includes/sci_pvm_barrier.h \
includes/sci_pvm_bufinfo.h \
includes/sci_pvm_halt.h \
includes/sci_pvm_start.h \
includes/sci_pvm_error_mode.h \
includes/sci_pvm_lvgroup.h \
includes/sci_pvm_send_var.h \
includes/sci_pvm_config.h \
includes/sci_pvm_recv_var.h

libscipvm_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/

lib_LTLIBRARIES = libscipvm.la

libscipvm_la_SOURCES = $(PVM_C_SOURCES) $(GATEWAY_C_SOURCES)

libscipvm_la_DEPENDENCIES = \
	$(top_builddir)/modules/core/libcore.la \
	$(top_builddir)/libs/MALLOC/libmalloc.la

libscipvm_la_LDFLAGS = $(PVM_LIB)



libscipvm_la_LIBADD = $(libscipvm_la_DEPENDENCIES)

libscipvm_la_PKGCONFIG = scipvm.pc
else
		echo "PVM not activated. Skip this module."
endif