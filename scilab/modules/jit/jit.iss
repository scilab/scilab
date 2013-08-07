;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Pierre MARECHAL
; Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
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
; JIT module
;--------------------------------------------------------------------------------------------------------------

#define JIT "jit"
#define JIT_TOOLS "jit-tools"
;
Source: bin\{#JIT}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#JIT}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#JIT_TOOLS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#JIT_TOOLS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#JIT}\etc\{#JIT}.quit; DestDir: {app}\modules\{#JIT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#JIT}\etc\{#JIT}.start; DestDir: {app}\modules\{#JIT}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#JIT}\macros\buildmacros.sce; DestDir: {app}\modules\{#JIT}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#JIT}\macros\buildmacros.bat; DestDir: {app}\modules\{#JIT}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#JIT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#JIT}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#JIT}\tests\*.*; DestDir: {app}\modules\{#JIT}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
