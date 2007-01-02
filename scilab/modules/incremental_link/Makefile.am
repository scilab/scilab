##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/incremental_link


#### incremental_link : Conf files ####
libincremental_link_la_rootdir = $(target)
libincremental_link_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### incremental_link : Scilab macros ###
libincremental_link_la_macrodir = $(target)/macros
libincremental_link_la_macro_DATA = macros/ilib_for_link.sci \
macros/G_make.sci \
macros/VCtoLCCLib.sci \
macros/findmsvccompiler.sci \
macros/ilib_unix_soname.sci \
macros/ilib_gen_Make.sci \
macros/ilib_compile.sci \
macros/ilib_mex_build.sci \
macros/configure_msvc.sci \
macros/ilib_gen_gateway.sci \
macros/with_lcc.sci \
macros/ilib_build.sci \
macros/ilib_gen_loader.sci \
macros/haveacompiler.sci macros/configure_msvc.bin \
macros/findmsvccompiler.bin \
macros/G_make.bin \
macros/haveacompiler.bin \
macros/ilib_build.bin \
macros/ilib_compile.bin \
macros/ilib_for_link.bin \
macros/ilib_gen_gateway.bin \
macros/ilib_gen_loader.bin \
macros/ilib_gen_Make.bin \
macros/ilib_mex_build.bin \
macros/ilib_unix_soname.bin \
macros/VCtoLCCLib.bin \
macros/with_lcc.bin macros/names macros/lib macros/buildmacros.sce


#### incremental_link : init scripts #### 
libincremental_link_la_etcdir = $(target)/etc
libincremental_link_la_etc_DATA = etc/incremental_link.quit etc/incremental_link.start

#### incremental_link : documentation FR #### 
libincremental_link_la_helpfrdir = $(target)/help/fr
libincremental_link_la_helpfr_DATA2 = help/fr/ilib_gen_gateway.htm \
help/fr/ilib_for_link.htm \
help/fr/findmsvccompiler.htm \
help/fr/ilib_gen_loader.htm \
help/fr/ilib_gen_Make.htm \
help/fr/scilink.htm \
help/fr/configure_msvc.htm \
help/fr/whatis.htm \
help/fr/G_make.htm \
help/fr/ilib_compile.htm \
help/fr/ilib_build.htm


#### incremental_link : documentation EN #### 
libincremental_link_la_helpendir = $(target)/help/eng
libincremental_link_la_helpen_DATA2 = help/eng/whatis.htm \
help/eng/findmsvccompiler.htm \
help/eng/configure_msvc.htm \
help/eng/G_make.htm \
help/eng/ilib_compile.htm \
help/eng/ilib_build.htm \
help/eng/ilib_gen_gateway.htm \
help/eng/ilib_for_link.htm \
help/eng/scilink.htm \
help/eng/ilib_gen_loader.htm \
help/eng/ilib_gen_Make.htm


#### incremental_link : compilation scripts #### 
libincremental_link_la_scriptdir = $(target)/src/scripts/
libincremental_link_la_script_DATA = src/scripts/Makefile.in \
src/scripts/configure.ac \
src/scripts/ltmain.sh \
src/scripts/Makefile.am \
src/scripts/aclocal.m4

libincremental_link_la_script_SCRIPTS = src/scripts/compilerDetection.sh \
src/scripts/config.guess \
src/scripts/config.status \
src/scripts/config.sub \
src/scripts/configure \
src/scripts/depcomp \
src/scripts/install-sh \
src/scripts/missing \
src/scripts/scicompile.sh \
src/scripts/libtool
include $(top_srcdir)/Makefile.incl.am
