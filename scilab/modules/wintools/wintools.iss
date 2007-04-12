;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; wintools module
;--------------------------------------------------------------------------------------------------------------
;
#define WINTOOLS "wintools"
;
Source: modules\{#WINTOOLS}\VERSION.xml; DestDir: {app}\modules\{#WINTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\readme.txt; DestDir: {app}\modules\{#WINTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\licence.txt; DestDir: {app}\modules\{#WINTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\changelog.txt; DestDir: {app}\modules\{#WINTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\sci_gateway\wintools_gateway.xml; DestDir: {app}\modules\{#WINTOOLS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\etc\{#WINTOOLS}.quit; DestDir: {app}\modules\{#WINTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\etc\{#WINTOOLS}.start; DestDir: {app}\modules\{#WINTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#WINTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\help\removechapter.sce; DestDir: {app}\modules\{#WINTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\help\*.htm; DestDir: {app}\modules\{#WINTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\languages\*.xml; DestDir: {app}\modules\{#WINTOOLS}\languages; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINTOOLS}\includes\*.h; DestDir: {app}\modules\{#WINTOOLS}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WINTOOLS}\macros\lib; DestDir: {app}\modules\{#WINTOOLS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#WINTOOLS}\macros\names; DestDir: {app}\modules\{#WINTOOLS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#WINTOOLS}\macros\*.bin; DestDir: {app}\modules\{#WINTOOLS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#WINTOOLS}\macros\*.sci; DestDir: {app}\modules\{#WINTOOLS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#WINTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#WINTOOLS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WINTOOLS}\demos\*.*; DestDir: {app}\modules\{#WINTOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
