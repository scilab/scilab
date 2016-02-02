;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
Source: modules\{#SIMANNEALING}\macros\*.sci; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\*.bin; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\tests\*.*; DestDir: {app}\modules\{#SIMANNEALING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
