;##############################################################################################################
; Script d'installation Inno Setup (5.2.2) pour scilab
; Allan CORNET
; Version TRUNK
; This file is released under the 3-clause BSD license. See COPYING-BSD.
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; toolbox_skeleton
;--------------------------------------------------------------------------------------------------------------
#define TOOLBOX_SKELETON "toolbox_skeleton"
;
Source: contrib\{#TOOLBOX_SKELETON}\builder.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\toolbox_skeleton_redist.iss; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\changelog.txt; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\license.txt; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\readme.txt; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\DESCRIPTION; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}; Components: {#COMPN_TOOLBOX_SKELETON}
;
Source: contrib\{#TOOLBOX_SKELETON}\demos\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\demos; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton.quit; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton.start; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton_preferences.xml; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton_preferences.xsl; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\help\builder_help.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\help; Components: {#COMPN_TOOLBOX_SKELETON}

Source: contrib\{#TOOLBOX_SKELETON}\help\en_US\build_help.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\help\en_US; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\help\en_US\*.xml; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\help\en_US; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\help\fr_FR\build_help.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\help\fr_FR; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\help\fr_FR\*.xml; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\help\fr_FR; Components: {#COMPN_TOOLBOX_SKELETON}
;
;Source: contrib\{#TOOLBOX_SKELETON}\includes\*.h; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\includes
;Source: contrib\{#TOOLBOX_SKELETON}\locales\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\locales
;
Source: contrib\{#TOOLBOX_SKELETON}\macros\buildmacros.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\macros; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\macros\cleanmacros.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\macros; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\macros\*.sci; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\macros; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\builder_gateway.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\builder_gateway_c.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_csum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_csub.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_multiplybypi.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_foo.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_cerror.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cpp_find.cxx; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cpperror.cpp; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cppfoo.cpp; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cppmultiplybypi.cpp; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cppsub.cpp; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cppsum.cpp; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\builder_gateway_cpp.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran\builder_gateway_fortran.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran\sci_fsum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\builder_src.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\builder_c.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\csum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\*.h; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\csub.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\multiplybypi.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\fortran\builder_fortran.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\fortran\fsum.f; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\tests\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\tests; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\locales\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\locales; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\java\builder_java.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\java; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\java\org\scilab\contrib\toolboxskeleton\Sum.java; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\java\org\scilab\contrib\toolboxskeleton; Components: {#COMPN_TOOLBOX_SKELETON}

;--------------------------------------------------------------------------------------------------------------
#define XCOS_TOOLBOX_SKELETON "xcos_toolbox_skeleton"
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\DESCRIPTION; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\license.txt; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\builder.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\changelog.txt; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\demos\*.*; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\demos; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\etc\{#XCOS_TOOLBOX_SKELETON}.quit; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\etc; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\etc\{#XCOS_TOOLBOX_SKELETON}.start; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\etc; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\examples\*.*; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\examples; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\help\builder_help.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\help; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\help\en_US\*.xml; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\help\en_US; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\help\en_US\build_help.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\help\en_US; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\help\fr_FR\build_help.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\help\fr_FR; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\images\gif\*.gif; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\images\gif; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\images\svg\*.svg; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\images\svg; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\macros\*.sci; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\macros; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\macros\*.mo; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\macros; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\macros\buildmacros.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\macros; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\macros\cleanmacros.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\macros; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\builder_gateway.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\c\builder_gateway_c.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\c\sci_tbx_sum.c; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway\loader_gateway.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\sci_gateway; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\builder_src.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\c\block_sum.c; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\c\builder_c.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\c\business.h; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\c\business_sum.c; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\c\tbx_block_noop.c; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src\c; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\src\cleaner_src.sce; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\src; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\unit_tests\tbx_sum.tst; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\unit_tests; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\unit_tests\tbx_sum.dia.ref; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\unit_tests; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_matrices.zcos; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_matrices_c.tst; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_matrices_scilab.tst; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_scalar.zcos; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_scalar_c.tst; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
Source: contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark\noop_scalar_scilab.tst; DestDir: {app}\contrib\{#XCOS_TOOLBOX_SKELETON}\tests\benchmark; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON} and {#COMPN_XCOS}
;--------------------------------------------------------------------------------------------------------------
