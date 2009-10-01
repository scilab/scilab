;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; string module
;--------------------------------------------------------------------------------------------------------------
;
#define STRING "string"
;
Source: bin\string.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\string_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\VERSION.xml; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\readme.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\license.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\changelog.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\sci_gateway\string_gateway.xml; DestDir: {app}\modules\{#STRING}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\etc\{#STRING}.quit; DestDir: {app}\modules\{#STRING}\etc; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\etc\{#STRING}.start; DestDir: {app}\modules\{#STRING}\etc; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\help\addchapter.sce; DestDir: {app}\modules\{#STRING}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#STRING}\includes\*.h; DestDir: {app}\modules\{#STRING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\macros\buildmacros.sce; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\buildmacros.bat; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\lib; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\names; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\*.bin; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\*.sci; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#STRING}\demos\*.*; DestDir: {app}\modules\{#STRING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\tests\*.*; DestDir: {app}\modules\{#STRING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
