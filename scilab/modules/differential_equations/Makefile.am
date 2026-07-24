# Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2006 - INRIA - Sylvestre LEDRU
# Copyright (C) 2023 - UTC - Stéphane MOTTELET
#
# This file is distributed under the same license as the Scilab package.

DIFFERENTIAL_EQUATIONS_CPP_SOURCES = \
    src/cpp/scifunctions.cpp \
    src/cpp/checkodeerror.cpp \
    src/cpp/differentialequationfunctions.cpp \
    src/cpp/SUNDIALSManager.cpp \
    src/cpp/CVODEManager.cpp \
    src/cpp/KINSOLManager.cpp \
    src/cpp/IDAManager.cpp \
    src/cpp/ARKODEManager.cpp \
    src/cpp/SPRKODEManager.cpp \
    src/cpp/OdeManagerInit.cpp \
    src/cpp/OdeManagerCompute.cpp \
    src/cpp/OdeManagerParse.cpp \
    src/cpp/OdeManagerHelpers.cpp \
    src/cpp/OdeManagerSolve.cpp \
    src/cpp/odeparameters.cpp \
    src/cpp/complexHelpers.cpp

DIFFERENTIAL_EQUATIONS_C_SOURCES = \
    src/c/colnewmsgs.c \
    src/c/dassl.c \
    src/c/feval.c \
    src/c/arnol.c \
    src/c/rk4.c \
    src/c/Ex-odedc.c \
    src/c/Ex-ode.c \
    src/c/Ex-daskr.c \
    src/c/errmds.c \
    src/c/externals_for_tests.c

DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES = \
    src/fortran/rscar1.f \
    src/fortran/bcomp.f \
    src/fortran/lcomp.f \
    src/fortran/loren.f \
    src/fortran/prja.f \
    src/fortran/vnorm.f \
    src/fortran/lsoda.f \
    src/fortran/lsodar.f \
    src/fortran/ainvg.f \
    src/fortran/lsode.f \
    src/fortran/svcom1.f \
    src/fortran/dqk21.f \
    src/fortran/solsy.f \
    src/fortran/lsodi.f \
    src/fortran/ddassl.f \
    src/fortran/dqpsrt.f \
    src/fortran/dqelg.f \
    src/fortran/cfode.f \
    src/fortran/rscma1.f \
    src/fortran/colnew.f \
    src/fortran/dcutet.f \
    src/fortran/xsetf.f \
    src/fortran/dgbsl.f \
    src/fortran/rkf45.f \
    src/fortran/rchek.f \
    src/fortran/dqags.f \
    src/fortran/xerrwv.f \
    src/fortran/twodq.f \
    src/fortran/dqagse.f \
    src/fortran/greatr.f \
    src/fortran/hpdel.f \
    src/fortran/hpins.f \
    src/fortran/svcar1.f \
    src/fortran/rscom1.f \
    src/fortran/rksimp.f \
    src/fortran/roots.f \
    src/fortran/stoda.f \
    src/fortran/bnorm.f \
    src/fortran/rchek2.f \
    src/fortran/stode.f \
    src/fortran/vmnorm.f \
    src/fortran/prepj.f \
    src/fortran/lsdisc.f \
    src/fortran/fnorm.f \
    src/fortran/ddasrt.f \
    src/fortran/ddaskr.f \
    src/fortran/daux.f \
    src/fortran/stodi.f \
    src/fortran/xsetun.f \
    src/fortran/intdy.f \
    src/fortran/ewset.f \
    src/fortran/roots2.f \
    src/fortran/prepji.f \
    src/fortran/svcma1.f \
    src/fortran/lsrgk.f \
    src/fortran/odeint.f \
    src/fortran/rkqc.f \
    src/fortran/writbufode.f

GATEWAY_FORTRAN_SOURCES = \
    sci_gateway/fortran/Ex-impl.f \
    sci_gateway/fortran/Ex-int2d.f \
    sci_gateway/fortran/Ex-int3d.f \
    sci_gateway/fortran/Ex-intg.f \
    sci_gateway/fortran/Ex-dasrt.f \
    sci_gateway/fortran/Ex-daskr.f \
    sci_gateway/fortran/Ex-dassl.f \
    sci_gateway/fortran/Ex-bvode.f

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/differential_equations_gw.cpp \
    sci_gateway/cpp/sci_int2d.cpp \
    sci_gateway/cpp/sci_int3d.cpp \
    sci_gateway/cpp/sci_intg.cpp \
    sci_gateway/cpp/sci_ode.cpp \
    sci_gateway/cpp/sci_odedc.cpp \
    sci_gateway/cpp/sci_feval.cpp \
    sci_gateway/cpp/sci_bvode.cpp \
    sci_gateway/cpp/sci_impl.cpp \
    sci_gateway/cpp/sci_dassl.cpp \
    sci_gateway/cpp/sci_dasrt.cpp \
    sci_gateway/cpp/sci_daskr.cpp \
    sci_gateway/cpp/sci_cvode.cpp \
    sci_gateway/cpp/sci_ida.cpp \
    sci_gateway/cpp/sci_arkode.cpp \
    sci_gateway/cpp/sci_sprkode.cpp \
    sci_gateway/cpp/sci_kinsol.cpp \
    sci_gateway/cpp/sci_percent_odeSolution.cpp

libscidifferential_equations_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/patched_sundials/include/ \
    -I$(srcdir)/src/patched_sundials/src/ \
    -I$(srcdir)/src/patched_sundials/src/sundials \
    -I$(top_srcdir)/modules/sparse/includes/ \
    -I$(top_srcdir)/modules/sparse/src/ColPack/includes/ \
    -I$(top_srcdir)/modules/sparse/src/ColPack/include/ \
    -I$(top_srcdir)/modules/sparse/src/ColPack/src/Utilities/ \
    -I$(top_srcdir)/modules/sparse/src/ColPack/src/Recovery/ \
    -I$(top_srcdir)/modules/sparse/src/ColPack/src/GeneralGraphColoring \
    -I$(top_srcdir)/modules/sparse/src/ColPack/src/BipartiteGraphPartialColoring \
    -I$(top_srcdir)/modules/sparse/src/ColPack/src/BipartiteGraphBicoloring \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    $(AM_CPPFLAGS) \
    $(EIGEN_CPPFLAGS) \
    $(OPENMP_CXXFLAGS)


#### scicos : SUNDIALS STUFF ####
libscisundials_la_SOURCES = \
    src/patched_sundials/src/arkode/arkode.c \
    src/patched_sundials/src/arkode/arkode_user_controller.c \
    src/patched_sundials/src/arkode/arkode_sunstepper.c \
    src/patched_sundials/src/arkode/arkode_sprkstep.c \
    src/patched_sundials/src/arkode/arkode_sprkstep_io.c \
    src/patched_sundials/src/arkode/arkode_sprk.c \
    src/patched_sundials/src/arkode/arkode_splittingstep.c \
    src/patched_sundials/src/arkode/arkode_splittingstep_coefficients.c \
    src/patched_sundials/src/arkode/arkode_root.c \
    src/patched_sundials/src/arkode/arkode_relaxation.c \
    src/patched_sundials/src/arkode/arkode_mristep.c \
    src/patched_sundials/src/arkode/arkode_mristep_nls.c \
    src/patched_sundials/src/arkode/arkode_mristep_io.c \
    src/patched_sundials/src/arkode/arkode_mristep_controller.c \
    src/patched_sundials/src/arkode/arkode_mri_tables.c \
    src/patched_sundials/src/arkode/arkode_lsrkstep.c \
    src/patched_sundials/src/arkode/arkode_lsrkstep_io.c \
    src/patched_sundials/src/arkode/arkode_ls.c \
    src/patched_sundials/src/arkode/arkode_io.c \
    src/patched_sundials/src/arkode/arkode_interp.c \
    src/patched_sundials/src/arkode/arkode_forcingstep.c \
    src/patched_sundials/src/arkode/arkode_erkstep.c \
    src/patched_sundials/src/arkode/arkode_erkstep_io.c \
    src/patched_sundials/src/arkode/arkode_butcher.c \
    src/patched_sundials/src/arkode/arkode_butcher_erk.c \
    src/patched_sundials/src/arkode/arkode_butcher_dirk.c \
    src/patched_sundials/src/arkode/arkode_bbdpre.c \
    src/patched_sundials/src/arkode/arkode_bandpre.c \
    src/patched_sundials/src/arkode/arkode_arkstep.c \
    src/patched_sundials/src/arkode/arkode_arkstep_nls.c \
    src/patched_sundials/src/arkode/arkode_arkstep_io.c \
    src/patched_sundials/src/arkode/arkode_adapt.c \
    src/patched_sundials/src/cvodes/cvodea.c \
    src/patched_sundials/src/cvodes/cvodea_io.c \
    src/patched_sundials/src/cvodes/cvodes.c \
    src/patched_sundials/src/cvodes/cvodes_bandpre.c \
    src/patched_sundials/src/cvodes/cvodes_bbdpre.c \
    src/patched_sundials/src/cvodes/cvodes_diag.c \
    src/patched_sundials/src/cvodes/cvodes_io.c \
    src/patched_sundials/src/cvodes/cvodes_ls.c \
    src/patched_sundials/src/cvodes/cvodes_nls.c \
    src/patched_sundials/src/cvodes/cvodes_nls_sim.c \
    src/patched_sundials/src/cvodes/cvodes_nls_stg.c \
    src/patched_sundials/src/cvodes/cvodes_nls_stg1.c \
    src/patched_sundials/src/cvodes/cvodes_proj.c \
    src/patched_sundials/src/idas/idaa.c \
    src/patched_sundials/src/idas/idaa_io.c \
    src/patched_sundials/src/idas/idas.c \
    src/patched_sundials/src/idas/idas_bbdpre.c \
    src/patched_sundials/src/idas/idas_ic.c \
    src/patched_sundials/src/idas/idas_io.c \
    src/patched_sundials/src/idas/idas_ls.c \
    src/patched_sundials/src/idas/idas_nls.c \
    src/patched_sundials/src/idas/idas_nls_sim.c \
    src/patched_sundials/src/idas/idas_nls_stg.c \
    src/patched_sundials/src/kinsol/kinsol.c \
    src/patched_sundials/src/kinsol/kinsol_bbdpre.c \
    src/patched_sundials/src/kinsol/kinsol_io.c \
    src/patched_sundials/src/kinsol/kinsol_ls.c \
    src/patched_sundials/src/nvector/openmp/nvector_openmp.c \
    src/patched_sundials/src/nvector/serial/nvector_serial.c \
    src/patched_sundials/src/nvector/manyvector/nvector_manyvector.c \
    src/patched_sundials/src/sunadaptcontroller/imexgus/sunadaptcontroller_imexgus.c \
    src/patched_sundials/src/sunadaptcontroller/mrihtol/sunadaptcontroller_mrihtol.c \
    src/patched_sundials/src/sunadaptcontroller/soderlind/sunadaptcontroller_soderlind.c \
    src/patched_sundials/src/sundials/sundials_adaptcontroller.c \
    src/patched_sundials/src/sundials/sundials_adjointcheckpointscheme.c \
    src/patched_sundials/src/sundials/sundials_band.c \
    src/patched_sundials/src/sundials/sundials_context.c \
    src/patched_sundials/src/sundials/sundials_dense.c \
    src/patched_sundials/src/sundials/sundials_direct.c \
    src/patched_sundials/src/sundials/sundials_errors.c \
    src/patched_sundials/src/sundials/sundials_extension.c \
    src/patched_sundials/src/sundials/sundials_futils.c \
    src/patched_sundials/src/sundials/sundials_hashmap.c \
    src/patched_sundials/src/sundials/sundials_iterative.c \
    src/patched_sundials/src/sundials/sundials_linearsolver.c \
    src/patched_sundials/src/sundials/sundials_logger.c \
    src/patched_sundials/src/sundials/sundials_math.c \
    src/patched_sundials/src/sundials/sundials_matrix.c \
    src/patched_sundials/src/sundials/sundials_memory.c \
    src/patched_sundials/src/sundials/sundials_nonlinearsolver.c \
    src/patched_sundials/src/sundials/sundials_nvector.c \
    src/patched_sundials/src/sundials/sundials_nvector_senswrapper.c \
    src/patched_sundials/src/sundials/sundials_stepper.c \
    src/patched_sundials/src/sundials/sundials_adjointstepper.c \
    src/patched_sundials/src/sundials/sundials_version.c \
    src/patched_sundials/src/sunlinsol/band/sunlinsol_band.c \
    src/patched_sundials/src/sunlinsol/dense/sunlinsol_dense.c \
    src/patched_sundials/src/sunlinsol/lapackband/sunlinsol_lapackband.c \
    src/patched_sundials/src/sunlinsol/lapackdense/sunlinsol_lapackdense.c \
    src/patched_sundials/src/sunlinsol/pcg/sunlinsol_pcg.c \
    src/patched_sundials/src/sunlinsol/klu/sunlinsol_klu.c \
    src/patched_sundials/src/sunlinsol/spbcgs/sunlinsol_spbcgs.c \
    src/patched_sundials/src/sunlinsol/spfgmr/sunlinsol_spfgmr.c \
    src/patched_sundials/src/sunlinsol/spgmr/sunlinsol_spgmr.c \
    src/patched_sundials/src/sunlinsol/sptfqmr/sunlinsol_sptfqmr.c \
    src/patched_sundials/src/sunmatrix/band/sunmatrix_band.c \
    src/patched_sundials/src/sunmatrix/dense/sunmatrix_dense.c \
    src/patched_sundials/src/sunmatrix/sparse/sunmatrix_sparse.c \
    src/patched_sundials/src/sunmemory/system/sundials_system_memory.c \
    src/patched_sundials/src/sunnonlinsol/fixedpoint/sunnonlinsol_fixedpoint.c \
    src/patched_sundials/src/sunnonlinsol/newton/sunnonlinsol_newton.c

libscisundials_la_CPPFLAGS = -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/cvodes \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/idas \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/arkode \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/kinsol \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/nvector \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/sundials \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/sunmatrix \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/sunlinsol \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/include/sunnonlinsol \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/src/sundials \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/src/cvodes \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/src/idas \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/src/arkode \
    -I$(top_srcdir)/modules/differential_equations/src/patched_sundials/src/kinsol \
    $(OPENMP_CFLAGS) \
    $(KLU_CFLAGS)

libscisundials_la_LIBADD = $(KLU_LIBS) \
    $(UMFPACK_LIBS) \
    $(OPENMP_LIBS)

pkglib_LTLIBRARIES = libscisundials.la libscidifferential_equations.la
if MAINTAINER_MODE
noinst_LTLIBRARIES = libscidifferential_equations-algo.la
endif

if IS_MACOSX
# enforce -O0 for some files
src/fortran/colnew.lo: src/fortran/colnew.f
	$(LIBTOOL) $(AM_V_lt)  --tag=F77 $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(F77) $(AM_FFLAGS) $(FFLAGS) -O0 -c -o src/fortran/colnew.lo src/fortran/colnew.f
endif

libscidifferential_equations_algo_la_SOURCES = $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES) $(DIFFERENTIAL_EQUATIONS_CPP_SOURCES)
libscidifferential_equations_la_SOURCES = $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_CPP_SOURCES) $(GATEWAY_C_SOURCES)
libscidifferential_equations_algo_la_CPPFLAGS = $(libscidifferential_equations_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_CPP_SOURCES)
INCLUDE_FLAGS = $(libscidifferential_equations_la_CPPFLAGS)

libscidifferential_equations_la_LIBADD = libscidifferential_equations-algo.la \
    libscisundials.la \
    $(KLU_LIBS) \
    $(UMFPACK_LIBS) \
    $(OPENMP_LIBS) \
    $(FLIBS)


libscidifferential_equations_la_includedir=$(pkgincludedir)
libscidifferential_equations_la_include_HEADERS =

#### Target ######
modulename=differential_equations


#### differential_equations : Conf files ####
libscidifferential_equations_la_rootdir = $(mydatadir)
libscidifferential_equations_la_root_DATA =  license.txt

#### differential_equations : init scripts ####
libscidifferential_equations_la_etcdir = $(mydatadir)/etc
libscidifferential_equations_la_etc_DATA = etc/differential_equations.quit etc/differential_equations.start

#### sundials : include files ####
# find src/patched_sundials/include  -type f -name '*.h' -or -name '*.hxx' -or -name '*.hpp' | sort |xargs printf '%s \\\n'

libscidifferential_equations_nvector_la_includedir=$(includedir)/nvector/
libscidifferential_equations_nvector_la_include_HEADERS = \
src/patched_sundials/include/nvector/nvector_serial.h \
src/patched_sundials/include/nvector/nvector_manyvector.h \
src/patched_sundials/include/nvector/nvector_openmp.h

libscidifferential_equations_sundials_la_includedir=$(includedir)/sundials/
libscidifferential_equations_sundials_la_include_HEADERS = \
src/patched_sundials/include/sundials/sundials_base.hpp \
src/patched_sundials/include/sundials/sundials_config.h \
src/patched_sundials/include/sundials/sundials_context.h \
src/patched_sundials/include/sundials/sundials_context.hpp \
src/patched_sundials/include/sundials/sundials_convertibleto.hpp \
src/patched_sundials/include/sundials/sundials_export.h \
src/patched_sundials/include/sundials/sundials_logger.h \
src/patched_sundials/include/sundials/sundials_matrix.h \
src/patched_sundials/include/sundials/sundials_matrix.hpp \
src/patched_sundials/include/sundials/sundials_memory.h \
src/patched_sundials/include/sundials/sundials_nvector.h \
src/patched_sundials/include/sundials/sundials_nvector.hpp \
src/patched_sundials/include/sundials/sundials_types.h \
src/patched_sundials/include/sundials/sundials_version.h \
src/patched_sundials/include/sundials/sundials_errors.h

libscidifferential_equations_sundialspriv_la_includedir=$(includedir)/sundials/priv/
libscidifferential_equations_sundialspriv_la_include_HEADERS = \
src/patched_sundials/include/sundials/priv/sundials_context_impl.h \
src/patched_sundials/include/sundials/priv/sundials_errors_impl.h


libscidifferential_equations_sunmatrix_la_includedir=$(includedir)/sunmatrix/
libscidifferential_equations_sunmatrix_la_include_HEADERS = \
src/patched_sundials/include/sunmatrix/sunmatrix_band.h \
src/patched_sundials/include/sunmatrix/sunmatrix_dense.h \
src/patched_sundials/include/sunmatrix/sunmatrix_sparse.h

#### differential_equations : gateway declaration ####
libscidifferential_equations_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidifferential_equations_la_sci_gateway_DATA = sci_gateway/differential_equations_gateway.xml

include $(top_srcdir)/Makefile.incl.am
