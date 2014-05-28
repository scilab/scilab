;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2014 - Scilab Enterprises
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
; symbol module
;--------------------------------------------------------------------------------------------------------------
;
#define SYMBOL "symbol"
;
Source: bin\{#SYMBOL}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SYMBOL}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SYMBOL}\includes\*.*; DestDir: {app}\modules\{#SYMBOL}\includes; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
