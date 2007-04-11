;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; cacsd module
;--------------------------------------------------------------------------------------------------------------
;
#define CACSD "cacsd"
;
Source: bin\slicot_f.dll; DestDir: {app}\bin; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\VERSION; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}; Check: set_with_cacsd
Source: modules\{#CACSD}\readme.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\licence.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\changelog.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\sci_gateway\cacsd_gateway.xml; DestDir: {app}\modules\{#CACSD}\sci_gateway; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\etc\{#CACSD}.quit; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\etc\{#CACSD}.start; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\help\addchapter.sce; DestDir: {app}\modules\{#CACSD}\help; Flags: recursesubdirs; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\help\removechapter.sce; DestDir: {app}\modules\{#CACSD}\help; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\help\*.htm; DestDir: {app}\modules\{#CACSD}\help; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\languages\*.xml; DestDir: {app}\modules\{#CACSD}\languages; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\includes\*.h; DestDir: {app}\modules\{#CACSD}\includes; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\macros\lib; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\names; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\*.bin; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\*.sci; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\buildmacros.sce; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\demos\*.*; DestDir: {app}\modules\{#CACSD}\demos; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
;--------------------------------------------------------------------------------------------------------------
