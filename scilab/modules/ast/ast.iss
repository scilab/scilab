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
; AST module
;--------------------------------------------------------------------------------------------------------------

#define AST "ast"
#define AST_TOOLS "ast-tools"
;
Source: bin\{#AST}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#AST}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\includes\*.*; DestDir: {app}\modules\{#AST}\includes; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\etc\{#AST}.quit; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\etc\{#AST}.start; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\macros\buildmacros.sce; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\buildmacros.bat; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\cleanmacros.bat; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\lib; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\*.sci; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\*.bin; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\tests\*.*; DestDir: {app}\modules\{#AST}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
