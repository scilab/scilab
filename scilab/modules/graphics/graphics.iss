;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; graphics module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPHICS "graphics"
;
Source: modules\{#GRAPHICS}\VERSION.xml; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\readme.txt; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\license.txt; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\changelog.txt; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
;
Source: thirdparty\jlatexmath-0.3.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
;
Source: bin\graphics.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;
Source: bin\nographics.dll; DestDir: {app}\bin; DestName: graphics.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: modules\{#GRAPHICS}\sci_gateway\graphics_gateway.xml; DestDir: {app}\modules\{#GRAPHICS}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.quit; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.start; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\help\addchapter.sce; DestDir: {app}\modules\{#GRAPHICS}\help; Flags: recursesubdirs; Components:  {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHICS}\includes\*.h; DestDir: {app}\modules\{#GRAPHICS}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\lib; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\names; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.sci; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.bin; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\tcl\*.*; DestDir: {app}\modules\{#GRAPHICS}\tcl; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\demos\*.*; DestDir: {app}\modules\{#GRAPHICS}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\tests\*.*; DestDir: {app}\modules\{#GRAPHICS}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
