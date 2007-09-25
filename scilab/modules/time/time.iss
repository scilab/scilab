;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; time module
;--------------------------------------------------------------------------------------------------------------
;
#define TIME "time"
;
Source: bin\time.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\time.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\changelog.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\licence.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\readme.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\VERSION.xml; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\includes\*.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\sci_gateway\time_gateway.xml; DestDir: {app}\modules\{#TIME}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\etc\{#TIME}.quit; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\etc\{#TIME}.start; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\help\addchapter.sce; DestDir: {app}\modules\{#TIME}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#TIME}\help\removechapter.sce; DestDir: {app}\modules\{#TIME}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\help\*.htm; DestDir: {app}\modules\{#TIME}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\locales\*.xml; DestDir: {app}\modules\{#TIME}\locales; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\macros\buildmacros.sce; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.bin; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.sci; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\lib; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\names; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
