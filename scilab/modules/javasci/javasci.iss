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
Source: modules\{#JAVASCI}\license.txt; DestDir: {app}\modules\{#JAVASCI}; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\readme.txt; DestDir: {app}\modules\{#JAVASCI}; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\VERSION.xml; DestDir: {app}\modules\{#JAVASCI}; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\changelog.txt; DestDir: {app}\modules\{#JAVASCI}; Components: {#COMPN_JAVASCI}
;
Source: bin\Javasci.dll; DestDir: {app}\bin; Components: {#COMPN_JAVASCI}
Source: modules\{#JAVASCI}\jar\javasci.jar;DestDir: {app}\modules\{#JAVASCI}\jar; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\help\addchapter.sce; DestDir: {app}\modules\{#JAVASCI}\help; Flags: recursesubdirs; Components:  {#COMPN_JAVASCI}
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
;Source: modules\{#JAVASCI}\includes\*.h; DestDir: {app}\modules\{#JAVASCI}\includes; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\src\java\javasci\*.*; DestDir: {app}\modules\{#JAVASCI}\src\java\javasci; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\examples\*.*; DestDir: {app}\modules\{#JAVASCI}\examples; Flags: recursesubdirs; Components: {#COMPN_JAVASCI}
;
Source: modules\{#JAVASCI}\tests\*.*; DestDir: {app}\modules\{#JAVASCI}\tests; Flags: recursesubdirs; Components: {#COMPN_JAVASCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
