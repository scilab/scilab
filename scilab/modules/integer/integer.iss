;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; integer module
;--------------------------------------------------------------------------------------------------------------
;
#define INTEGER "integer"
;
Source: bin\integer_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\VERSION.xml; DestDir: {app}\modules\{#INTEGER}; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\readme.txt; DestDir: {app}\modules\{#INTEGER}; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\licence.txt; DestDir: {app}\modules\{#INTEGER}; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\changelog.txt; DestDir: {app}\modules\{#INTEGER}; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\sci_gateway\integer_gateway.xml; DestDir: {app}\modules\{#INTEGER}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\etc\{#INTEGER}.quit; DestDir: {app}\modules\{#INTEGER}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\etc\{#INTEGER}.start; DestDir: {app}\modules\{#INTEGER}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\help\addchapter.sce; DestDir: {app}\modules\{#INTEGER}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#INTEGER}\help\removechapter.sce; DestDir: {app}\modules\{#INTEGER}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\help\*.htm; DestDir: {app}\modules\{#INTEGER}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\locales\*.xml; DestDir: {app}\modules\{#INTEGER}\locales; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\includes\*.h; DestDir: {app}\modules\{#INTEGER}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\lib; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\names; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\*.bin; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\*.sci; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INTEGER}\demos\*.*; DestDir: {app}\modules\{#INTEGER}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
