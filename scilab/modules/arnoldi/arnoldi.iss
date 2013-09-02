;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; arnoldi module
;--------------------------------------------------------------------------------------------------------------
;
#define ARNOLDI "arnoldi"
;
Source: bin\{#ARNOLDI}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB};
;Source: bin\no{#ARNOLDI}.dll; DestDir: {app}\bin; DestName: arnoldi.dll; Components: not ({#COMPN_SCILAB})
;
Source: bin\Arpack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\license.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\sci_gateway\{#ARNOLDI}_gateway.xml; DestDir: {app}\modules\{#ARNOLDI}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.quit; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.start; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_SCILAB}
;
;
;Source: modules\{#ARNOLDI}\includes\*.h; DestDir: {app}\modules\{#ARNOLDI}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\macros\lib; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\names; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\*.bin; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\*.sci; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\buildmacros.sce; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\buildmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ARNOLDI}\demos\*.*; DestDir: {app}\modules\{#ARNOLDI}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\tests\*.*; DestDir: {app}\modules\{#ARNOLDI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
