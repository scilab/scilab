;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; SCI2C module
;--------------------------------------------------------------------------------------------------------------
;
#define SCI2C "sci2c"
;
Source: modules\{#SCI2C}\VERSION.xml; DestDir: {app}\modules\{#SCI2C}; Components: {#COMPN_SCI2C}; Check: set_with_SCI2C
Source: modules\{#SCI2C}\readme.txt; DestDir: {app}\modules\{#SCI2C}; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\licence.txt; DestDir: {app}\modules\{#SCI2C}; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\changelog.txt; DestDir: {app}\modules\{#SCI2C}; Components: {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\sci_gateway\SCI2C; DestDir: {app}\modules\{#SCI2C}\sci_gateway; Components: {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\etc\{#SCI2C}.quit; DestDir: {app}\modules\{#SCI2C}\etc; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\etc\{#SCI2C}.start; DestDir: {app}\modules\{#SCI2C}\etc; Components: {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\help\addchapter.sce; DestDir: {app}\modules\{#SCI2C}\help; Flags: recursesubdirs; Components:  {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\help\*.htm; DestDir: {app}\modules\{#SCI2C}\help; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;
;
Source: modules\{#SCI2C}\includes\*.h; DestDir: {app}\modules\{#SCI2C}\includes; Components: {#COMPN_SCI2C}
;
;Source: modules\{#SCI2C}\macros\lib; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;Source: modules\{#SCI2C}\macros\names; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;Source: modules\{#SCI2C}\macros\*.bin; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;Source: modules\{#SCI2C}\macros\*.sci; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\macros\buildmacros.bat; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
Source: modules\{#SCI2C}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SCI2C}\macros; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\demos\*.*; DestDir: {app}\modules\{#SCI2C}\demos; Flags: recursesubdirs; Components: {#COMPN_SCI2C}
;
Source: modules\{#SCI2C}\tests\*.*; DestDir: {app}\modules\{#SCI2C}\tests; Flags: recursesubdirs; Components: {#COMPN_SCI2C} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
