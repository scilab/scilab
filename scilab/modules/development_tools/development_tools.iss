;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; development_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define DEVTOOLS "development_tools"
;
Source: modules\{#DEVTOOLS}\VERSION.xml; DestDir: {app}\modules\{#DEVTOOLS}; Components: {#COMPN_DEVTOOLS}; Check: set_with_{#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\readme.txt; DestDir: {app}\modules\{#DEVTOOLS}; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\licence.txt; DestDir: {app}\modules\{#DEVTOOLS}; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\changelog.txt; DestDir: {app}\modules\{#DEVTOOLS}; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\etc\{#DEVTOOLS}.quit; DestDir: {app}\modules\{#DEVTOOLS}\etc; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\etc\{#DEVTOOLS}.start; DestDir: {app}\modules\{#DEVTOOLS}\etc; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#DEVTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\help\removechapter.sce; DestDir: {app}\modules\{#DEVTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\help\*.htm; DestDir: {app}\modules\{#DEVTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\locales\*.xml; DestDir: {app}\modules\{#DEVTOOLS}\locales; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\includes\*.h; DestDir: {app}\modules\{#DEVTOOLS}\includes; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\macros\lib; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\names; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\demos\*.*; DestDir: {app}\modules\{#DEVTOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\examples\*.*; DestDir: {app}\modules\{#DEVTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
;--------------------------------------------------------------------------------------------------------------
