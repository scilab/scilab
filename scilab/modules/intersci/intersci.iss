;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; intersci module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERSCI "intersci"
;
Source: modules\{#INTERSCI}\VERSION.xml; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}; Check: set_with_intersci
Source: modules\{#INTERSCI}\readme.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\licence.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\changelog.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.quit; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.start; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\help\addchapter.sce; DestDir: {app}\modules\{#INTERSCI}\help; Flags: recursesubdirs; Components:  {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\help\*.htm; DestDir: {app}\modules\{#INTERSCI}\help; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
;
;Source: modules\{#INTERSCI}\macros\lib; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\names; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.bin; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.sci; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\demos\*.*; DestDir: {app}\modules\{#INTERSCI}\demos; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\bin\*.exe; DestDir: {app}\modules\{#INTERSCI}\bin; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\examples\*.*; DestDir: {app}\modules\{#INTERSCI}\examples; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\tests\*.*; DestDir: {app}\modules\{#INTERSCI}\tests; Flags: recursesubdirs; Components: {#COMPN_INTERSCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
