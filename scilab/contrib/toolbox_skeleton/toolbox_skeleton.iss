;##############################################################################################################
; Script d'installation Inno Setup (5.2.2) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2008
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
;
Source: contrib\{#TOOLBOX_SKELETON}\demos\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\demos; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton.quit; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\etc\toolbox_skeleton.start; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\etc; Components: {#COMPN_TOOLBOX_SKELETON}
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
Source: contrib\{#TOOLBOX_SKELETON}\macros\*.sci; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\macros; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\builder_gateway.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\builder_gateway_c.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_csum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\c\sci_csub.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\sci_cpp_find.cxx; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp\builder_gateway_cpp.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\cpp; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran\builder_gateway_fortran.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran\sci_fsum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\sci_gateway\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\builder_src.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\builder_c.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\csum.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\c\csub.c; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\c; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\fortran\builder_fortran.sce; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\src\fortran\fsum.f; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\src\fortran; Components: {#COMPN_TOOLBOX_SKELETON}
Source: contrib\{#TOOLBOX_SKELETON}\tests\*.*; DestDir: {app}\contrib\{#TOOLBOX_SKELETON}\tests; Flags: recursesubdirs; Components: {#COMPN_TOOLBOX_SKELETON}
;--------------------------------------------------------------------------------------------------------------
