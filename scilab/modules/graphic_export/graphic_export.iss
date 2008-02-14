;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2008
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; graphic_export module
;--------------------------------------------------------------------------------------------------------------
;
; TO DO : a component "graphics_export"
;
#define GRAPHIC_EXPORT "graphic_export"
;
Source: bin\gl2ps.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\scigraphic_export.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\nographic_export.dll; DestDir: {app}\bin; DestName: scigraphic_export.dll; Components: not ({#COMPN_GRAPHICS})
;
;
Source: modules\{#GRAPHIC_EXPORT}\jar\org.scilab.modules.graphic_export.jar;DestDir: {app}\modules\{#GRAPHIC_EXPORT}\jar; Components: {#COMPN_GRAPHICS}
;

Source: modules\{#GRAPHIC_EXPORT}\changelog.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS};
Source: modules\{#GRAPHIC_EXPORT}\license.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\readme.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\version.xml; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\sci_gateway\graphic_export_gateway.xml; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\includes\*.h; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\help\addchapter.sce; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\help; Flags: recursesubdirs; Components:  {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\etc\{#GRAPHIC_EXPORT}.quit; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\etc\{#GRAPHIC_EXPORT}.start; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\help\*.htm; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\help; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
;
Source: modules\{#GRAPHIC_EXPORT}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#GRAPHIC_EXPORT}\macros\*.bin; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#GRAPHIC_EXPORT}\macros\*.sci; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#GRAPHIC_EXPORT}\macros\lib; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#GRAPHIC_EXPORT}\macros\names; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHIC_EXPORT}\demos\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHIC_EXPORT}\tests\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
