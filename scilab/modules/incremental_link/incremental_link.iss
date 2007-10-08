;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; incremental_link module
;--------------------------------------------------------------------------------------------------------------
;
#define INCRLINK "incremental_link"
;
Source: modules\{#INCRLINK}\VERSION.xml; DestDir: {app}\modules\{#INCRLINK}; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\readme.txt; DestDir: {app}\modules\{#INCRLINK}; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\licence.txt; DestDir: {app}\modules\{#INCRLINK}; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\changelog.txt; DestDir: {app}\modules\{#INCRLINK}; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\etc\{#INCRLINK}.quit; DestDir: {app}\modules\{#INCRLINK}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\etc\{#INCRLINK}.start; DestDir: {app}\modules\{#INCRLINK}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\help\addchapter.sce; DestDir: {app}\modules\{#INCRLINK}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\help\removechapter.sce; DestDir: {app}\modules\{#INCRLINK}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\help\*.htm; DestDir: {app}\modules\{#INCRLINK}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\locales\*.xml; DestDir: {app}\modules\{#INCRLINK}\locales; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\src\scripts\Makedll.incl; DestDir: {app}\modules\{#INCRLINK}\src\scripts; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INCRLINK}\includes\*.h; DestDir: {app}\modules\{#INCRLINK}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\macros\lib; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\names; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\*.bin; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\*.sci; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\buildmacros.sce; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\buildmacros.bat; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INCRLINK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INCRLINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INCRLINK}\demos\*.*; DestDir: {app}\modules\{#INCRLINK}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INCRLINK}\examples\*.*; DestDir: {app}\modules\{#INCRLINK}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
