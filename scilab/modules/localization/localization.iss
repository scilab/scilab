;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; localization module
;--------------------------------------------------------------------------------------------------------------
;
#define LOCALIZATION "localization"
;
Source: modules\{#LOCALIZATION}\VERSION; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\readme.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\licence.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\changelog.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.quit; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.start; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\help\en\*.htm; DestDir: {app}\modules\{#LOCALIZATION}\help\en; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\help\fr\*.htm; DestDir: {app}\modules\{#LOCALIZATION}\help\fr; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\includes\*.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\macros\lib; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\macros\names; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\macros\*.bin; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\macros\*.sci; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\xml\*.xml; DestDir: {app}\modules\{#LOCALIZATION}\xml; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\demos\*.*; DestDir: {app}\modules\{#LOCALIZATION}\demos; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
