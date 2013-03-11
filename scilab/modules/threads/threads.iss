;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2011 - Allan CORNET
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
; threads module
;--------------------------------------------------------------------------------------------------------------
;
#define THREADS "threads"
;
Source: bin\{#THREADS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#THREADS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#THREADS}\includes\dynlib_threads.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#THREADS}\includes\Thread_Wrapper.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
