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
;
#define GENETICALGORITHMS "genetic_algorithms"
;
Source: modules\{#GENETICALGORITHMS}\VERSION.xml; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\readme.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\license.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\changelog.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.quit; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.start; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\demos\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\macros\lib; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\names; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.bin; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.sci; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\tests\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
