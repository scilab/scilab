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
;
#define GENETICALGORITHMS "genetic_algorithms"
;
Source: modules\{#GENETICALGORITHMS}\license.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.quit; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.start; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\demos\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\macros\lib; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.sci; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.bin; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\tests\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
