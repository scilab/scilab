;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
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
Source: bin\arnoldi.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI}; Check: set_with_arnoldi
Source: bin\Arpack.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI}
Source: bin\UMFPACK.dll; DestDir: {app}\bin; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\VERSION; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\readme.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\licence.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\changelog.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\sci_gateway\arnoldi; DestDir: {app}\modules\{#ARNOLDI}\sci_gateway; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.quit; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.start; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#ARNOLDI}\help\en_US; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#ARNOLDI}\help\en_US; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#ARNOLDI}\help\fr_FR; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#ARNOLDI}\help\fr_FR; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\help\en_US\*.htm; DestDir: {app}\modules\{#ARNOLDI}\help\en_US; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\help\fr_FR\*.htm; DestDir: {app}\modules\{#ARNOLDI}\help\fr_FR; Components: {#COMPN_ARNOLDI}
;
Source: modules\{#ARNOLDI}\languages\en_US\*.xml; DestDir: {app}\modules\{#ARNOLDI}\languages\en_US; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#ARNOLDI}\languages\fr_FR; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\includes\*.h; DestDir: {app}\modules\{#ARNOLDI}\includes; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\macros\lib; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\names; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\*.bin; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;Source: modules\{#ARNOLDI}\macros\*.sci; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
Source: modules\{#ARNOLDI}\macros\buildmacros.sce; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_ARNOLDI}
;
;Source: modules\{#ARNOLDI}\demos\*.*; DestDir: {app}\modules\{#ARNOLDI}\demos; Components: {#COMPN_ARNOLDI}
;
;--------------------------------------------------------------------------------------------------------------
