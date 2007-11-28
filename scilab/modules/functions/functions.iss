;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; functions module
;--------------------------------------------------------------------------------------------------------------
;
#define FUNCTIONS "functions"
;
Source: modules\{#FUNCTIONS}\VERSION.xml; DestDir: {app}\modules\{#FUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\readme.txt; DestDir: {app}\modules\{#FUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\licence.txt; DestDir: {app}\modules\{#FUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\changelog.txt; DestDir: {app}\modules\{#FUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\etc\{#FUNCTIONS}.quit; DestDir: {app}\modules\{#FUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\etc\{#FUNCTIONS}.start; DestDir: {app}\modules\{#FUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\help\*.htm; DestDir: {app}\modules\{#FUNCTIONS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#FUNCTIONS}\help\addchapter.sce; DestDir: {app}\modules\{#FUNCTIONS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\macros\lib; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\names; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\xml\*.dtd; DestDir: {app}\modules\{#FUNCTIONS}\xml; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#FUNCTIONS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\scripts\buildmacros\*.sce; DestDir: {app}\modules\{#FUNCTIONS}\scripts\buildmacros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\scripts\buildmacros\*.bat; DestDir: {app}\modules\{#FUNCTIONS}\scripts\buildmacros; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\examples\*.*; DestDir: {app}\modules\{#FUNCTIONS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\nonreg_tests\*.*; DestDir: {app}\modules\{#FUNCTIONS}\nonreg_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_NO_REGRESSION_TESTS}
;
Source: modules\{#FUNCTIONS}\unit_tests\*.ref; DestDir: {app}\modules\{#FUNCTIONS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_UNIT_TESTS}
Source: modules\{#FUNCTIONS}\unit_tests\*.tst; DestDir: {app}\modules\{#FUNCTIONS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_UNIT_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
