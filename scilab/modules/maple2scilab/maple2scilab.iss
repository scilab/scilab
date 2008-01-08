;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; maple2scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define MAP2SCI "maple2scilab"
;
Source: modules\{#MAP2SCI}\VERSION.xml; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\readme.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\licence.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\changelog.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.quit; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.start; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\addchapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help; Flags: recursesubdirs; Components:  {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\*.htm; DestDir: {app}\modules\{#MAP2SCI}\help; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI}
;
;
Source: modules\{#MAP2SCI}\src\maple\*.mpl; DestDir: {app}\modules\{#MAP2SCI}\src\maple; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\macros\lib; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\names; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.bin; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.sci; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
;
;Source: modules\{#MAP2SCI}\demos\*.*; DestDir: {app}\modules\{#MAP2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\tests\*.*; DestDir: {app}\modules\{#MAP2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
