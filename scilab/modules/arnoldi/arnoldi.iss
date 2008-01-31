;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; arnoldi module
;--------------------------------------------------------------------------------------------------------------
;
#define ARNOLDI "arnoldi"
;
Source: bin\arnoldi.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI};
;Source: bin\noarnoldi.dll; DestDir: {app}\bin; DestName: arnoldi.dll; Components: not ({#COMPN_ARNOLDI})
;
Source: bin\Arpack.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI}
Source: bin\UMFPACK.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\VERSION.xml; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\readme.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\license.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\changelog.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\sci_gateway\arnoldi_gateway.xml; DestDir: {app}\modules\{#ARNOLDI}\sci_gateway; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.quit; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.start; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\help\addchapter.sce; DestDir: {app}\modules\{#ARNOLDI}\help; Flags: recursesubdirs; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\help\*.htm; DestDir: {app}\modules\{#ARNOLDI}\help; Flags: recursesubdirs; Components: {#COMPN_ARNOLDI}
;
;
;Source: modules\{#ARNOLDI}\includes\*.h; DestDir: {app}\modules\{#ARNOLDI}\includes; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\macros\lib; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\names; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\*.bin; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\*.sci; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\macros\buildmacros.sce; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\macros\buildmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\demos\*.*; DestDir: {app}\modules\{#ARNOLDI}\demos; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\tests\*.*; DestDir: {app}\modules\{#ARNOLDI}\tests; Flags: recursesubdirs; Components: {#COMPN_ARNOLDI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
