;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; javasci module
;--------------------------------------------------------------------------------------------------------------
;
#define JAVASCI "javasci"
;
Source: bin\Javasci.dll; DestDir: {app}\bin; Components: {#COMPN_JAVASCI}; Check: set_with_javasci
Source: modules\{#JAVASCI}\jar\javasci.jar;DestDir: {app}\modules\{#JAVASCI}\jar; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\help\addchapter.sce; DestDir: {app}\modules\{#JAVASCI}\help; Flags: recursesubdirs; Components:  {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\help\*.htm; DestDir: {app}\modules\{#JAVASCI}\help; Flags: recursesubdirs; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\help\images\*.gif; DestDir: {app}\modules\{#JAVASCI}\help\images; Components: {#COMPN_JAVASCI}
;
;
Source: modules\{#JAVASCI}\etc\{#JAVASCI}.quit; DestDir: {app}\modules\{#JAVASCI}\etc; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\etc\{#JAVASCI}.start; DestDir: {app}\modules\{#JAVASCI}\etc; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
;Source: modules\{#JAVASCI}\macros\*.bin; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
;Source: modules\{#JAVASCI}\macros\*.sci; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
;Source: modules\{#JAVASCI}\macros\lib; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
;Source: modules\{#JAVASCI}\macros\names; DestDir: {app}\modules\{#JAVASCI}\macros; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\includes\*.h; DestDir: {app}\modules\{#JAVASCI}\includes; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\src\java\javasci\*.*; DestDir: {app}\modules\{#JAVASCI}\src\java\javasci; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\examples\*.*; DestDir: {app}\modules\{#JAVASCI}\examples; Flags: recursesubdirs; Components: {#COMPN_JAVASCI}
;
;Source: modules\{#JAVASCI}\nonreg_tests\*.*; DestDir: {app}\modules\{#JAVASCI}\nonreg_tests; Flags: recursesubdirs; Components: {#COMPN_JAVASCI} and {#COMPN_NO_REGRESSION_TESTS}
;
;Source: modules\{#JAVASCI}\unit_tests\*.ref; DestDir: {app}\modules\{#JAVASCI}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_JAVASCI} and {#COMPN_UNIT_TESTS}
;Source: modules\{#JAVASCI}\unit_tests\*.tst; DestDir: {app}\modules\{#JAVASCI}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_JAVASCI} and {#COMPN_UNIT_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
