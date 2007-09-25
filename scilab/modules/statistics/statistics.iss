;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; statistics module
;--------------------------------------------------------------------------------------------------------------
;
#define STATS "statistics"
;
Source: modules\{#STATS}\VERSION.xml; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}; Check: set_with_statistics
Source: modules\{#STATS}\readme.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
Source: modules\{#STATS}\licence.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
Source: modules\{#STATS}\changelog.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\sci_gateway\statistics_gateway.xml; DestDir: {app}\modules\{#STATS}\sci_gateway; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\etc\{#STATS}.quit; DestDir: {app}\modules\{#STATS}\etc; Components: {#COMPN_STATS}
Source: modules\{#STATS}\etc\{#STATS}.start; DestDir: {app}\modules\{#STATS}\etc; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\help\addchapter.sce; DestDir: {app}\modules\{#STATS}\help; Flags: recursesubdirs; Components:  {#COMPN_STATS}
Source: modules\{#STATS}\help\removechapter.sce; DestDir: {app}\modules\{#STATS}\help; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\help\*.htm; DestDir: {app}\modules\{#STATS}\help; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\locales\*.xml; DestDir: {app}\modules\{#STATS}\locales; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\macros\lib; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\names; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\*.bin; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\*.sci; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\buildmacros.sce; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
;
;Source: modules\{#STATS}\demos\*.*; DestDir: {app}\modules\{#STATS}\demos; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
;--------------------------------------------------------------------------------------------------------------
