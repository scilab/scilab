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
; sound module
;--------------------------------------------------------------------------------------------------------------
;
#define SOUND "sound"
;
Source: bin\sound.dll; DestDir: {app}\bin; Components: {#COMPN_SOUND}
;
Source: bin\nosound.dll; DestDir: {app}\bin; DestName: sound.dll; Components: not ({#COMPN_SOUND})
;
Source: modules\{#SOUND}\changelog.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\license.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\readme.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\VERSION.xml; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\sci_gateway\sound_gateway.xml; DestDir: {app}\modules\{#SOUND}\sci_gateway; Components: {#COMPN_SOUND}
;
;Source: modules\{#SOUND}\includes\*.h; DestDir: {app}\modules\{#SOUND}\includes; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\etc\{#SOUND}.quit; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\etc\{#SOUND}.start; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\help\addchapter.sce; DestDir: {app}\modules\{#SOUND}\help; Flags: recursesubdirs; Components:  {#COMPN_SOUND}
;
Source: modules\{#SOUND}\macros\buildmacros.sce; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\buildmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.bin; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.sci; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\lib; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\names; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\demos\*.*; DestDir: {app}\modules\{#SOUND}\demos; Flags: recursesubdirs; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\tests\*.*; DestDir: {app}\modules\{#SOUND}\tests; Flags: recursesubdirs; Components: {#COMPN_SOUND} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
