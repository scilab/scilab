;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; sparse module
;--------------------------------------------------------------------------------------------------------------
;
#define SPARSE "sparse"
;
Source: bin\sparse_f.dll; DestDir: {app}\bin; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\VERSION.xml; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\readme.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\license.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\changelog.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\sci_gateway\sparse_gateway.xml; DestDir: {app}\modules\{#SPARSE}\sci_gateway; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\etc\{#SPARSE}.quit; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\etc\{#SPARSE}.start; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\help\addchapter.sce; DestDir: {app}\modules\{#SPARSE}\help; Flags: recursesubdirs; Components:  {#COMPN_SPARSE}
;
;
;Source: modules\{#SPARSE}\includes\*.h; DestDir: {app}\modules\{#SPARSE}\includes; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\lib; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\names; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\*.bin; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\*.sci; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
;
;Source: modules\{#SPARSE}\demos\*.*; DestDir: {app}\modules\{#SPARSE}\demos; Flags: recursesubdirs;  Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\tests\*.*; DestDir: {app}\modules\{#SPARSE}\tests; Flags: recursesubdirs; Components: {#COMPN_SPARSE} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
