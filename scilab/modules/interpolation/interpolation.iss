;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; interpolation module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERPOLATION "interpolation"
;
Source: modules\{#INTERPOLATION}\VERSION; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}; Check: set_with_interpolation
Source: modules\{#INTERPOLATION}\readme.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\licence.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\changelog.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\sci_gateway\interpolation_gateway.xml; DestDir: {app}\modules\{#INTERPOLATION}\sci_gateway; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.quit; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.start; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\help\addchapter.sce; DestDir: {app}\modules\{#INTERPOLATION}\help; Flags: recursesubdirs; Components:  {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\help\removechapter.sce; DestDir: {app}\modules\{#INTERPOLATION}\help; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\help\*.htm; DestDir: {app}\modules\{#INTERPOLATION}\help; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\languages\*.xml; DestDir: {app}\modules\{#INTERPOLATION}\languages; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\includes\*.h; DestDir: {app}\modules\{#INTERPOLATION}\includes; Components: {#COMPN_INTERP}
;
;Source: modules\{#INTERPOLATION}\macros\lib; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;Source: modules\{#INTERPOLATION}\macros\names; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;Source: modules\{#INTERPOLATION}\macros\*.bin; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;Source: modules\{#INTERPOLATION}\macros\*.sci; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\demos\*.*; DestDir: {app}\modules\{#INTERPOLATION}\demos; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
;--------------------------------------------------------------------------------------------------------------
