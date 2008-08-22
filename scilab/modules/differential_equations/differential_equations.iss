;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; differential_equations module
;--------------------------------------------------------------------------------------------------------------
;
#define DIFFEQU "differential_equations"
;
Source: modules\{#DIFFEQU}\VERSION.xml; DestDir: {app}\modules\{#DIFFEQU}; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\readme.txt; DestDir: {app}\modules\{#DIFFEQU}; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\license.txt; DestDir: {app}\modules\{#DIFFEQU}; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\changelog.txt; DestDir: {app}\modules\{#DIFFEQU}; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\sci_gateway\differential_equations_gateway.xml; DestDir: {app}\modules\{#DIFFEQU}\sci_gateway; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\etc\{#DIFFEQU}.quit; DestDir: {app}\modules\{#DIFFEQU}\etc; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\etc\{#DIFFEQU}.start; DestDir: {app}\modules\{#DIFFEQU}\etc; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\help\addchapter.sce; DestDir: {app}\modules\{#DIFFEQU}\help; Flags: recursesubdirs; Components:  {#COMPN_EQUDIF}
;
;Source: modules\{#DIFFEQU}\includes\*.h; DestDir: {app}\modules\{#DIFFEQU}\includes; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\macros\lib; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\names; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\*.bin; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\*.sci; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\buildmacros.sce; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\buildmacros.bat; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
Source: modules\{#DIFFEQU}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\demos\*.*; DestDir: {app}\modules\{#DIFFEQU}\demos; Flags: recursesubdirs; Components: {#COMPN_EQUDIF}
;
Source: modules\{#DIFFEQU}\tests\*.*; DestDir: {app}\modules\{#DIFFEQU}\tests; Flags: recursesubdirs; Components: {#COMPN_EQUDIF} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
