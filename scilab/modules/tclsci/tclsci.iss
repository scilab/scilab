;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; tclsci module
;--------------------------------------------------------------------------------------------------------------
;
#define TCLSCI "tclsci"
;
Source: modules\{#TCLSCI}\VERSION; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}; Check: set_with_tclsci
Source: modules\{#TCLSCI}\readme.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\licence.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\changelog.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\sci_gateway\tclsci; DestDir: {app}\modules\{#TCLSCI}\sci_gateway; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.quit; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.start; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\demos\sciGUI\*.*; DestDir: {app}\modules\{#TCLSCI}\demos\sciGUI; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\demos\tk\*.*; DestDir: {app}\modules\{#TCLSCI}\demos\tk; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#TCLSCI}\help\en_US; Components:  {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#TCLSCI}\help\en_US; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#TCLSCI}\help\fr_FR; Components:  {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#TCLSCI}\help\fr_FR; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\help\en_US\*.htm; DestDir: {app}\modules\{#TCLSCI}\help\en_US; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\help\fr_FR\*.htm; DestDir: {app}\modules\{#TCLSCI}\help\fr_FR; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\languages\en_US\*.xml; DestDir: {app}\modules\{#TCLSCI}\languages\en_US; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#TCLSCI}\languages\fr_FR; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#TCLSCI}\includes; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\macros\lib; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\names; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\*.bin; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\*.sci; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
Source: modules\{#TCLSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_TCLSCI}
;
Source: modules\{#TCLSCI}\tcl\*.*; DestDir: {app}\modules\{#TCLSCI}\tcl; Flags: recursesubdirs; Components: {#COMPN_TCLSCI}
;
Source: bin\notclsci.dll; DestDir: {app}\bin; DestName: tclsci.dll; Components: not ({#COMPN_TCLSCI})
;
Source: bin\tclsci.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tcl84.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tcl84.lib; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tk84.dll; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
Source: bin\tk84.lib; DestDir: {app}\bin; Components: {#COMPN_TCLSCI}
;
;--------------------------------------------------------------------------------------------------------------
