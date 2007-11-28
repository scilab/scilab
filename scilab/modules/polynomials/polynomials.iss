;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; polynomials module
;--------------------------------------------------------------------------------------------------------------
;
#define POLYNOMIALS "polynomials"
;
Source: modules\{#POLYNOMIALS}\VERSION.xml; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}; Check: set_with_polynomials
Source: modules\{#POLYNOMIALS}\readme.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\licence.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\changelog.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\sci_gateway\polynomials_gateway.xml; DestDir: {app}\modules\{#POLYNOMIALS}\sci_gateway; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.quit; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.start; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\addchapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help; Flags: recursesubdirs; Components:  {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\*.htm; DestDir: {app}\modules\{#POLYNOMIALS}\help\; Flags: recursesubdirs; Components: {#COMPN_POLY}
;
;
Source: modules\{#POLYNOMIALS}\includes\*.h; DestDir: {app}\modules\{#POLYNOMIALS}\includes; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\macros\buildmacros.sce; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\buildmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\lib; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\names; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.bin; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.sci; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
;
;Source: modules\{#POLYNOMIALS}\demos\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\demos; Flags: recursesubdirs; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\unit_tests\*.ref; DestDir: {app}\modules\{#POLYNOMIALS}\unit_tests; Flags: recursesubdirs; Components:  {#COMPN_POLY} and {#COMPN_UNIT_TESTS}
Source: modules\{#POLYNOMIALS}\unit_tests\*.tst; DestDir: {app}\modules\{#POLYNOMIALS}\unit_tests; Flags: recursesubdirs; Components:  {#COMPN_POLY} and {#COMPN_UNIT_TESTS}
;
Source: modules\{#POLYNOMIALS}\nonreg_tests\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\nonreg_tests; Flags: recursesubdirs; Components: {#COMPN_POLY} and {#COMPN_NO_REGRESSION_TESTS}
;
Source: modules\{#POLYNOMIALS}\unit_tests\*.ref; DestDir: {app}\modules\{#POLYNOMIALS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_POLY} and {#COMPN_UNIT_TESTS}
Source: modules\{#POLYNOMIALS}\unit_tests\*.tst; DestDir: {app}\modules\{#POLYNOMIALS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_POLY} and {#COMPN_UNIT_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
