; ##############################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - Pierre MARECHAL
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
; ##############################################################################
;-------------------------------------------------------------------------------
; atoms module
;-------------------------------------------------------------------------------
;
#define ATOMS "atoms"
;
Source: modules\{#ATOMS}\changelog.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\license.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\readme.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\version.xml; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\etc\{#ATOMS}.quit; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\etc\{#ATOMS}.start; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\help\addchapter.sce; DestDir: {app}\modules\{#ATOMS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\*.bin; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\*.sci; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\lib; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\names; DestDir: {app}\modules\{#ATOMS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\tests\*.*; DestDir: {app}\modules\{#ATOMS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;