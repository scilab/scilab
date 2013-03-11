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
; AST module
;--------------------------------------------------------------------------------------------------------------

#define AST "ast"
#define AST_TOOLS "ast-tools"
;
Source: bin\{#AST}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#AST}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#AST_TOOLS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#AST_TOOLS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\etc\{#AST}.quit; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\etc\{#AST}.start; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\macros\buildmacros.sce; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\buildmacros.bat; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\cleanmacros.bat; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\tests\*.*; DestDir: {app}\modules\{#AST}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
