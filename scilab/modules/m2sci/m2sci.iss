;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; m2sci module
;--------------------------------------------------------------------------------------------------------------
;
#define M2SCI "m2sci"
;
Source: modules\{#M2SCI}\VERSION.xml; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}; Check: set_with_m2sci
Source: modules\{#M2SCI}\readme.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\licence.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\changelog.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\etc\{#M2SCI}.quit; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\etc\{#M2SCI}.start; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\help\addchapter.sce; DestDir: {app}\modules\{#M2SCI}\help; Flags: recursesubdirs; Components:  {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\help\*.htm; DestDir: {app}\modules\{#M2SCI}\help; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
;
;
Source: modules\{#M2SCI}\tcl\*.tcl; DestDir: {app}\modules\{#M2SCI}\tcl; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\macros\lib; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\names; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\*.bin; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\*.sci; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
;
;Source: modules\{#M2SCI}\demos\*.*; DestDir: {app}\modules\{#M2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\tests\*.*; DestDir: {app}\modules\{#M2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_M2SCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
