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
Source: modules\{#OVERLOADING}\VERSION.xml; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\readme.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\licence.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\changelog.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.quit; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.start; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\help\addchapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\help\removechapter.sce; DestDir: {app}\modules\{#OVERLOADING}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\help\*.htm; DestDir: {app}\modules\{#OVERLOADING}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
;Source: modules\{#OVERLOADING}\includes\*.h; DestDir: {app}\modules\{#OVERLOADING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\macros\buildmacros.sce; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\buildmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\lib; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\names; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.bin; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.sci; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\demos\*.*; DestDir: {app}\modules\{#OVERLOADING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
