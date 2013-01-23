;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; api_scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define PARSE "parse"
;
Source: bin\{#PARSE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#PARSE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARSE}\etc\{#PARSE}.quit; DestDir: {app}\modules\{#PARSE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PARSE}\etc\{#PARSE}.start; DestDir: {app}\modules\{#PARSE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARSE}\macros\buildmacros.sce; DestDir: {app}\modules\{#PARSE}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARSE}\tests\*.*; DestDir: {app}\modules\{#PARSE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
