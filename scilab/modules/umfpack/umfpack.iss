;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2008
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; UMFPACK module
;--------------------------------------------------------------------------------------------------------------
;
#define UMFPACK "umfpack"
;
Source: bin\noumfpack.dll; DestDir: {app}\bin; DestName: umfpack.dll; Components: not ({#COMPN_UMFPACK})
Source: bin\libumfpack.dll; DestDir: {app}\bin; Components: {#COMPN_UMFPACK}
Source: bin\umfpack.dll; DestDir: {app}\bin; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\VERSION.xml; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\readme.txt; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\license.txt; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\changelog.txt; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\sci_gateway\umfpack_gateway.xml; DestDir: {app}\modules\{#UMFPACK}\sci_gateway; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.quit; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.start; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\help\addchapter.sce; DestDir: {app}\modules\{#UMFPACK}\help; Flags: recursesubdirs; Components:  {#COMPN_UMFPACK}
;
;Source: modules\{#UMFPACK}\includes\*.h; DestDir: {app}\modules\{#UMFPACK}\includes; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\macros\buildmacros.sce; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\buildmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\lib; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\names; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\*.bin; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\*.sci; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\examples\*.*; DestDir: {app}\modules\{#UMFPACK}\examples; Flags: recursesubdirs;  Components: {#COMPN_UMFPACK}
;
;Source: modules\{#UMFPACK}\demos\*.*; DestDir: {app}\modules\{#UMFPACK}\demos; Flags: recursesubdirs;  Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\tests\*.*; DestDir: {app}\modules\{#UMFPACK}\tests; Flags: recursesubdirs; Components: {#COMPN_UMFPACK} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
