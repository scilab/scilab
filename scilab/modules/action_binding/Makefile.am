##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### Vincent COUVERT
### INRIA - Scilab 2007
##########

# List of the c files
CONNECTORS_C_SOURCES = src/c/InterpreterManagement.c

# List of the gateway c files
CONSOLE_C_JNI_SOURCES = src/jni/InterpreterManagement_wrap.c

# Includes need for the compilation
libsciaction_binding_la_CFLAGS= $(JAVA_JNI_INCLUDE) -Iincludes/ -Isrc/c/ -Isrc/jni/ -I$(top_srcdir)/libs/MALLOC/includes/

# Name of the future library (.la will be replace by .so, .a, etc regarding 
# the system)
pkglib_LTLIBRARIES = libsciaction_binding.la

# All the sources needed by libsciaction_binding.la
libsciaction_binding_la_SOURCES = $(CONNECTORS_C_SOURCES) $(CONSOLE_C_JNI_SOURCES) 

## Libraries necessary
## Change to include other Scilab modules/lib
libsciaction_binding_la_LIBADD = \
	$(top_builddir)/libs/blas/libsciblas.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la

#### Name of the module and the path ######
modulename=action_binding

#### action_binding : Conf files ####
libsciaction_binding_la_rootdir = $(mydatadir)
libsciaction_binding_la_root_DATA = changelog.txt licence.txt readme.txt version.xml

#### action_binding : init scripts #### 
libsciaction_binding_la_etcdir = $(mydatadir)/etc
libsciaction_binding_la_etc_DATA = etc/action_binding.quit etc/action_binding.start

#### SWIG Declaration ####
SWIG_WRAPPERS = src/jni/InterpreterManagement.i

#### action_binding : gateway declaration #### 
libsciaction_binding_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciaction_binding_la_sci_gateway_DATA =

#### action_binding : include files ####
libsciaction_binding_la_includedir=$(pkgincludedir)/action_binding
libsciaction_binding_la_include_HEADERS = 

# Provides macros compilation, Java compilation, cleaning
# If you want Makefile to call ant, added USEANT=1
include $(top_srcdir)/Makefile.incl.am
