;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; tclsci module
;--------------------------------------------------------------------------------------------------------------
;
#define TCLSCI "tclsci"
;
Source: modules\{#TCLSCI}\VERSION.xml; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\readme.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\license.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\changelog.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\sci_gateway\tclsci_gateway.xml; DestDir: {app}\modules\{#TCLSCI}\sci_gateway; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.quit; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.start; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\demos\*.*; DestDir: {app}\modules\{#TCLSCI}\demos; Flags: recursesubdirs; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\help\addchapter.sce; DestDir: {app}\modules\{#TCLSCI}\help; Flags: recursesubdirs; Components:  {#COMPN_TCLSCI}
;
;Source: modules\{#TCLSCI}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#TCLSCI}\includes; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\macros\lib; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\names; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\*.bin; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\*.sci; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\tcl\*.*; DestDir: {app}\modules\{#TCLSCI}\tcl; Flags: recursesubdirs; Components: {#COMPN_TCLSCI}
;
Source: bin\notclsci.dll; DestDir: {app}\bin; DestName: tclsci.dll; Components: not ({#COMPN_TCLSCI})
;
Source: bin\tclsci.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tcl85.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tcl85.lib; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tk85.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tk85.lib; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\tests\*.*; DestDir: {app}\modules\{#TCLSCI}\tests; Flags: recursesubdirs; Components:  {#COMPN_TCLSCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
