;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; elementary_functions module
;--------------------------------------------------------------------------------------------------------------
;
#define ELEMFUNCTIONS "elementary_functions"
;
Source: bin\slatec_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\linpack_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\linpack_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\eispack_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\VERSION.xml; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\readme.txt; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\license.txt; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\changelog.txt; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\sci_gateway\elementary_functions_gateway.xml; DestDir: {app}\modules\{#ELEMFUNCTIONS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\etc\{#ELEMFUNCTIONS}.quit; DestDir: {app}\modules\{#ELEMFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\etc\{#ELEMFUNCTIONS}.start; DestDir: {app}\modules\{#ELEMFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\help\addchapter.sce; DestDir: {app}\modules\{#ELEMFUNCTIONS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#ELEMFUNCTIONS}\includes\*.h; DestDir: {app}\modules\{#ELEMFUNCTIONS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\lib; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\names; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ELEMFUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#ELEMFUNCTIONS}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#ELEMFUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
