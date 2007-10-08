;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; scilab2fortran module
;--------------------------------------------------------------------------------------------------------------
;
#define SCI2FOR "scilab2fortran"
;
Source: modules\{#SCI2FOR}\VERSION.xml; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}; Check: set_with_scilab2fortran
Source: modules\{#SCI2FOR}\readme.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\licence.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\changelog.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\etc\{#SCI2FOR}.quit; DestDir: {app}\modules\{#SCI2FOR}\etc; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\etc\{#SCI2FOR}.start; DestDir: {app}\modules\{#SCI2FOR}\etc; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\help\addchapter.sce; DestDir: {app}\modules\{#SCI2FOR}\help; Flags: recursesubdirs; Components:  {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\help\removechapter.sce; DestDir: {app}\modules\{#SCI2FOR}\help; Flags: recursesubdirs; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\help\*.htm; DestDir: {app}\modules\{#SCI2FOR}\help; Flags: recursesubdirs; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\locales\*.xml; DestDir: {app}\modules\{#SCI2FOR}\locales; Flags: recursesubdirs; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\macros\lib; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\names; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\*.bin; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\*.sci; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\buildmacros.bat; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\demos\*.*; DestDir: {app}\modules\{#SCI2FOR}\demos; Flags: recursesubdirs; Components: {#COMPN_SCI2FOR}
;
;--------------------------------------------------------------------------------------------------------------
