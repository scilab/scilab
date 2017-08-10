;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Pierre MARECHAL
; Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; atoms module
;--------------------------------------------------------------------------------------------------------------
;
#define ATOMS "atoms"
;
Source: modules\{#ATOMS}\license.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\etc\{#ATOMS}.quit; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\etc\{#ATOMS}.start; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\etc\repositories; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\images\icons\16x16\emblems\*.png; DestDir: {app}\modules\{#ATOMS}\images\icons\16x16\emblems; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#ATOMS}\images\icons\16x16\status\*.png; DestDir: {app}\modules\{#ATOMS}\images\icons\16x16\status; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ATOMS}\macros\lib; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\*.sci; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\*.bin; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#ATOMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\gui\*.xml; DestDir: {app}\modules\{#ATOMS}\gui; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ATOMS}\demos\*.*; DestDir: {app}\modules\{#ATOMS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#ATOMS}\tests\*.*; DestDir: {app}\modules\{#ATOMS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
#include BinariesSourcePath + "\contrib\external_modules.iss"
;--------------------------------------------------------------------------------------------------------------
