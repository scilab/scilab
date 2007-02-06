;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################

;--------------------------------------------------------------------------------------------------------------
; overloading module
;--------------------------------------------------------------------------------------------------------------
;
#define OVERLOADING "overloading"
;
Source: modules\{#OVERLOADING}\VERSION; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\readme.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\licence.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\changelog.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.quit; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.start; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\help\en\addchapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help\en; Components:  {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\help\en\removechapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help\en; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\help\fr\addchapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help\fr; Components:  {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\help\fr\removechapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help\fr; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\help\en\*.htm; DestDir: {app}\modules\{#OVERLOADING}\help\en; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\help\fr\*.htm; DestDir: {app}\modules\{#OVERLOADING}\help\fr; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\includes\*.h; DestDir: {app}\modules\{#OVERLOADING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\macros\buildmacros.sce; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\lib; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\names; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.bin; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.sci; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\demos\*.*; DestDir: {app}\modules\{#OVERLOADING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
