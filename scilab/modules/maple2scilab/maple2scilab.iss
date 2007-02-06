;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; maple2scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define MAP2SCI "maple2scilab"
;
Source: modules\{#MAP2SCI}\VERSION; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}; Check: set_with_maple2scilab
Source: modules\{#MAP2SCI}\readme.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\licence.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\changelog.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.quit; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.start; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\en\addchapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help\en; Components:  {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\help\en\removechapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help\en; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\fr\addchapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help\fr; Components:  {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\help\fr\removechapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help\fr; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\en\*.htm; DestDir: {app}\modules\{#MAP2SCI}\help\en; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\help\fr\*.htm; DestDir: {app}\modules\{#MAP2SCI}\help\fr; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\src\*.mpl; DestDir: {app}\modules\{#MAP2SCI}\src; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\macros\lib; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\names; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.bin; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.sci; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
;
;Source: modules\{#MAP2SCI}\demos\*.*; DestDir: {app}\modules\{#MAP2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI}
;
;--------------------------------------------------------------------------------------------------------------
