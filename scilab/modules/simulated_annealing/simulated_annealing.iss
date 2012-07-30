;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; simulated_annealing module
;--------------------------------------------------------------------------------------------------------------
;
#define SIMANNEALING "simulated_annealing"
;
Source: modules\{#SIMANNEALING}\license.txt; DestDir: {app}\modules\{#SIMANNEALING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\etc\{#SIMANNEALING}.quit; DestDir: {app}\modules\{#SIMANNEALING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\etc\{#SIMANNEALING}.start; DestDir: {app}\modules\{#SIMANNEALING}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\demos\*.*; DestDir: {app}\modules\{#SIMANNEALING}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\macros\lib; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\names; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\*.bin; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\*.sci; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\tests\*.*; DestDir: {app}\modules\{#SIMANNEALING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
