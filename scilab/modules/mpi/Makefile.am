# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

MPI_CPP_SOURCES = \
    src/cpp/getOptionalComm.cpp

MPI_C_SOURCES = \
    src/c/serialization.c \
    src/c/deserialization.c

# List of the gateway c files
GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_mpi_init.c \
    sci_gateway/c/sci_mpi_finalize.c \
    sci_gateway/c/sci_mpi_comm_size.c \
    sci_gateway/c/sci_mpi_comm_rank.c \
    sci_gateway/c/sci_mpi_recv.c \
    sci_gateway/c/sci_mpi_send.c \
    sci_gateway/c/sci_mpi_irecv.c \
    sci_gateway/c/sci_mpi_isend.c \
    sci_gateway/c/sci_mpi_bcast.c \
    sci_gateway/c/sci_mpi_wait.c \
    sci_gateway/c/sci_mpi_get_processor_name.c

GATEWAY_CPP_SOURCES = \
    sci_gateway/cpp/sci_mpi_createComm.cpp

# Includes need for the compilation
# MPI code needs to be compile with the openmpi 'compiler' (it is a wrapper)
CC = $(OPENMPI_CC)
libscimpi_la_CPPFLAGS=  -Iincludes/ \
    -Isrc/c/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/string/includes \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    -I$(top_srcdir)/modules/output_stream/includes \
    -I$(top_srcdir)/modules/localization/includes \
    -I$(top_srcdir)/modules/api_scilab/includes \
    -I$(top_srcdir)/modules/elementary_functions/includes \
    $(OPENMPI_CFLAGS)

# libscimpi_la_LDFLAGS = $(LAMPI_LDFLAGS)
libscimpi_la_LIBADD = $(OPENMPI_LIBS)

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libscimpi.la

# All the sources needed by libscimpi.la
libscimpi_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(MPI_C_SOURCES) $(MPI_CPP_SOURCES)
#$(GATEWAY_FORTRAN_SOURCES) $(MPI_FORTRAN_SOURCES)


#### Target ######
modulename=mpi

#### mpi : Conf files ####
libscimpi_la_rootdir = $(mydatadir)
libscimpi_la_root_DATA = readme.txt

#### mpi : init scripts ####
libscimpi_la_etcdir = $(mydatadir)/etc
libscimpi_la_etc_DATA = etc/mpi.quit etc/mpi.start

#### mpi : gateway declaration ####
libscimpi_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscimpi_la_sci_gateway_DATA = sci_gateway/mpi_gateway.xml

#### mpi : include files ####
libscimpi_la_includedir=$(pkgincludedir)/mpi
libscimpi_la_include_HEADERS = includes/gw_mpi.h \
                               includes/dynlib_mpi.h \
                               includes/sciMpiComm.hxx

# Provides macros compilation, Java compilation, cleaning
include $(top_srcdir)/Makefile.incl.am
