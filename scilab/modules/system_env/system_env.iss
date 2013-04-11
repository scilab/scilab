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
#define SYSTEM_ENV "system_env"
;
Source: bin\{#SYSTEM_ENV}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SYSTEM_ENV}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SYSTEM_ENV}\includes\sci_warning.h; DestDir: {app}\modules\{#SYSTEM_ENV}\includes; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
