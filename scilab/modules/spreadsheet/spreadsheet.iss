;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; spreadsheet module
;--------------------------------------------------------------------------------------------------------------
;
#define SPREADSHEET "spreadsheet"
;
Source: bin\spreadsheet.dll; DestDir: {app}\bin; Components: {#COMPN_SPREADSHEET}
;
Source: bin\nospreadsheet.dll; DestDir: {app}\bin; DestName: spreadsheet.dll; Components: not ({#COMPN_SPREADSHEET})
;
Source: modules\{#SPREADSHEET}\changelog.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}; Check: set_with_spreadsheet
Source: modules\{#SPREADSHEET}\licence.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\readme.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\VERSION.xml; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\sci_gateway\spreadsheet_gateway.xml; DestDir: {app}\modules\{#SPREADSHEET}\sci_gateway; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\includes\*.h; DestDir: {app}\modules\{#SPREADSHEET}\includes; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.quit; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.start; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\help\addchapter.sce; DestDir: {app}\modules\{#SPREADSHEET}\help; Flags: recursesubdirs; Components:  {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\help\removechapter.sce; DestDir: {app}\modules\{#SPREADSHEET}\help; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\help\*.htm; DestDir: {app}\modules\{#SPREADSHEET}\help; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET}
;
;
Source: modules\{#SPREADSHEET}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\*.bin; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\*.sci; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\lib; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\names; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\demos\*.*; DestDir: {app}\modules\{#SPREADSHEET}\demos; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET}
;
;Source: modules\{#SPREADSHEET}\unit_tests\*.ref; DestDir: {app}\modules\{#SPREADSHEET}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET} and {#COMPN_DEVTOOLS}
;Source: modules\{#SPREADSHEET}\unit_tests\*.tst; DestDir: {app}\modules\{#SPREADSHEET}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET} and {#COMPN_DEVTOOLS}
;
;--------------------------------------------------------------------------------------------------------------
