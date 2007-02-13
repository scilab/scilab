;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
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
Source: modules\{#INTERSCI}\VERSION; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}; Check: set_with_intersci
Source: modules\{#INTERSCI}\readme.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\licence.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\changelog.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.quit; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.start; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#INTERSCI}\help\en_US; Components:  {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#INTERSCI}\help\en_US; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#INTERSCI}\help\fr_FR; Components:  {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#INTERSCI}\help\fr_FR; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\help\en_US\*.htm; DestDir: {app}\modules\{#INTERSCI}\help\en_US; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\help\fr_FR\*.htm; DestDir: {app}\modules\{#INTERSCI}\help\fr_FR; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\macros\lib; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\names; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.bin; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.sci; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\demos\*.*; DestDir: {app}\modules\{#INTERSCI}\demos; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\bin\*.exe; DestDir: {app}\modules\{#INTERSCI}\bin; Components: {#COMPN_INTERSCI}
;
;--------------------------------------------------------------------------------------------------------------
