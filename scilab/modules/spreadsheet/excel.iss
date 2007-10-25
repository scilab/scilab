;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; excel module
;--------------------------------------------------------------------------------------------------------------
;
#define EXCEL "excel"
;
Source: bin\excel.dll; DestDir: {app}\bin; Components: {#COMPN_EXCEL}
;
Source: bin\noexcel.dll; DestDir: {app}\bin; DestName: excel.dll; Components: not ({#COMPN_EXCEL})
;
Source: modules\{#EXCEL}\changelog.txt; DestDir: {app}\modules\{#EXCEL}; Components: {#COMPN_EXCEL}; Check: set_with_excel
Source: modules\{#EXCEL}\licence.txt; DestDir: {app}\modules\{#EXCEL}; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\readme.txt; DestDir: {app}\modules\{#EXCEL}; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\VERSION.xml; DestDir: {app}\modules\{#EXCEL}; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\sci_gateway\excel_gateway.xml; DestDir: {app}\modules\{#EXCEL}\sci_gateway; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\includes\*.h; DestDir: {app}\modules\{#EXCEL}\includes; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\etc\{#EXCEL}.quit; DestDir: {app}\modules\{#EXCEL}\etc; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\etc\{#EXCEL}.start; DestDir: {app}\modules\{#EXCEL}\etc; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\help\addchapter.sce; DestDir: {app}\modules\{#EXCEL}\help; Flags: recursesubdirs; Components:  {#COMPN_EXCEL}
Source: modules\{#EXCEL}\help\removechapter.sce; DestDir: {app}\modules\{#EXCEL}\help; Flags: recursesubdirs; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\help\*.htm; DestDir: {app}\modules\{#EXCEL}\help; Flags: recursesubdirs; Components: {#COMPN_EXCEL}
;
;
Source: modules\{#EXCEL}\macros\buildmacros.sce; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\buildmacros.bat; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\cleanmacros.bat; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\*.bin; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\*.sci; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\lib; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
Source: modules\{#EXCEL}\macros\names; DestDir: {app}\modules\{#EXCEL}\macros; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\demos\*.*; DestDir: {app}\modules\{#EXCEL}\demos; Flags: recursesubdirs; Components: {#COMPN_EXCEL}
;
Source: modules\{#EXCEL}\unit_tests\*.ref; DestDir: {app}\modules\{#EXCEL}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_EXCEL} and {#COMPN_DEVTOOLS}
Source: modules\{#EXCEL}\unit_tests\*.tst; DestDir: {app}\modules\{#EXCEL}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_EXCEL} and {#COMPN_DEVTOOLS}

;
;--------------------------------------------------------------------------------------------------------------
