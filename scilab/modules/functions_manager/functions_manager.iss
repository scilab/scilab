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
; functions_manager module
;--------------------------------------------------------------------------------------------------------------

#define FUNC_MAN "functions_manager"
;
Source: bin\{#FUNC_MAN}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FUNC_MAN}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
