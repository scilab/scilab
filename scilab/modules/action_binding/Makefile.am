# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2007-2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
# Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

if GUI

USEANT=1

# List of the c files
ACTION_BINDING_C_SOURCES = src/c/InterpreterManagement.c \
src/c/dynamic_menus.c

# List of the gateway c files
CONSOLE_C_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/InterpreterManagement.i
else
# List of the c files
ACTION_BINDING_C_SOURCES = src/noaction_binding/noaction_binding.c \
src/c/dynamic_menus.c
endif

if SWIG
BUILT_SOURCES=swig
endif


# Includes need for the compilation
libsciaction_binding_la_CFLAGS= 	$(JAVA_JNI_INCLUDE) \
-I$(srcdir)/includes/ \
-I$(srcdir)/src/c/ \
-I$(srcdir)/src/jni/ \
-I$(top_srcdir)/libs/MALLOC/includes/ \
-I$(top_srcdir)/modules/api_scilab/includes \
-I$(top_srcdir)/modules/output_stream/includes

# Name of the future library (.la will be replace by .so, .a, etc regarding
# the system)
pkglib_LTLIBRARIES = libsciaction_binding.la

libsciaction_binding_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

# All the sources needed by libsciaction_binding.la
libsciaction_binding_la_SOURCES = $(ACTION_BINDING_C_SOURCES) $(CONSOLE_C_JNI_SOURCES)

# For the code check (splint)
CHECK_SRC= $(ACTION_BINDING_C_SOURCES)
INCLUDE_FLAGS = $(libsciaction_binding_la_CFLAGS)

## Libraries necessary
## Change to include other Scilab modules/lib
libsciaction_binding_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la
#### Name of the module and the path ######
modulename=action_binding

#### action_binding : Conf files ####
libsciaction_binding_la_rootdir = $(mydatadir)
libsciaction_binding_la_root_DATA = changelog.txt license.txt readme.txt version.xml

####�action_binding : init scripts ####
libsciaction_binding_la_etcdir = $(mydatadir)/etc
libsciaction_binding_la_etc_DATA = etc/action_binding.quit etc/action_binding.start

####�action_binding : gateway declaration ####
libsciaction_binding_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciaction_binding_la_sci_gateway_DATA =

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
