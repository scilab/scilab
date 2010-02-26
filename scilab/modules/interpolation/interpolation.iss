; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007 - INRIA - Allan CORNET
;
; This file is distributed under the same license as the Scilab package.
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; interpolation module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERPOLATION "interpolation"
;
Source: bin\interpolation_f.dll; DestDir: {app}\bin; Components: {#COMPN_INTERP}
Source: bin\interpolation.dll; DestDir: {app}\bin; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\VERSION.xml; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\readme.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\license.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\changelog.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\sci_gateway\interpolation_gateway.xml; DestDir: {app}\modules\{#INTERPOLATION}\sci_gateway; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.quit; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.start; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\help\addchapter.sce; DestDir: {app}\modules\{#INTERPOLATION}\help; Flags: recursesubdirs; Components:  {#COMPN_INTERP}
;
;Source: modules\{#INTERPOLATION}\includes\*.h; DestDir: {app}\modules\{#INTERPOLATION}\includes; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\macros\lib; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\names; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\*.bin; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\*.sci; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
Source: modules\{#INTERPOLATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\demos\*.*; DestDir: {app}\modules\{#INTERPOLATION}\demos; Flags: recursesubdirs; Components: {#COMPN_INTERP}
;
Source: modules\{#INTERPOLATION}\tests\*.*; DestDir: {app}\modules\{#INTERPOLATION}\tests; Flags: recursesubdirs; Components:{#COMPN_INTERP} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
