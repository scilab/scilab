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
;Source: bin\{#THREADS}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: modules\{#THREADS}\license.txt; DestDir: {app}\modules\{#THREADS}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#THREADS}\sci_gateway\{#THREADS}_gateway.xml; DestDir: {app}\modules\{#THREADS}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#THREADS}\etc\{#THREADS}.quit; DestDir: {app}\modules\{#THREADS}\etc; Components: {#COMPN_SCILAB}
;Source: modules\{#THREADS}\etc\{#THREADS}.start; DestDir: {app}\modules\{#THREADS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#THREADS}\includes\dynlib_threads.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#THREADS}\includes\Thread_Wrapper.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#THREADS}\macros\buildmacros.sce; DestDir: {app}\modules\{#THREADS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#THREADS}\macros\buildmacros.bat; DestDir: {app}\modules\{#THREADS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#THREADS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#THREADS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#THREADS}\macros\lib; DestDir: {app}\modules\{#THREADS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#THREADS}\macros\*.sci; DestDir: {app}\modules\{#THREADS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#THREADS}\demos\*.*; DestDir: {app}\modules\{#THREADS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
;Source: modules\{#THREADS}\tests\*.*; DestDir: {app}\modules\{#THREADS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
