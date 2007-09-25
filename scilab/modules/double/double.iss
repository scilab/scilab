;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; double module
;--------------------------------------------------------------------------------------------------------------
;
#define DOUBLE "double"
;
Source: modules\{#DOUBLE}\VERSION.xml; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\readme.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\licence.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\changelog.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\etc\{#DOUBLE}.quit; DestDir: {app}\modules\{#DOUBLE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\etc\{#DOUBLE}.start; DestDir: {app}\modules\{#DOUBLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\help\addchapter.sce; DestDir: {app}\modules\{#DOUBLE}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\help\removechapter.sce; DestDir: {app}\modules\{#DOUBLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DOUBLE}\help\*.htm; DestDir: {app}\modules\{#DOUBLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\locales\*.xml; DestDir: {app}\modules\{#DOUBLE}\locales; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DOUBLE}\includes\*.h; DestDir: {app}\modules\{#DOUBLE}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\macros\buildmacros.sce; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\lib; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\names; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\*.bin; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\*.sci; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DOUBLE}\demos\*.*; DestDir: {app}\modules\{#DOUBLE}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
