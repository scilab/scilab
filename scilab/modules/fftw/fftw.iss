;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
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
; FFTW module
;--------------------------------------------------------------------------------------------------------------
;
#define FFTW "fftw"
;
Source: bin\{#FFTW}\lib{#FFTW}3-3.dll; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW}
Source: bin\{#FFTW}\COPYRIGHT; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW}
Source: bin\{#FFTW}\COPYING; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW}

Source: bin\{#FFTW}.dll; DestDir: {app}\bin; DestName: {#FFTW}.dll; Components: {#COMPN_FFTW}
Source: bin\no{#FFTW}.dll; DestDir: {app}\bin; DestName: {#FFTW}.dll; Components: not ({#COMPN_FFTW})
;

Source: modules\{#FFTW}\VERSION.xml; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\readme.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\license.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\changelog.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\sci_gateway\{#FFTW}_gateway.xml; DestDir: {app}\modules\{#FFTW}\sci_gateway; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\etc\{#FFTW}.quit; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\etc\{#FFTW}.start; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\demos\*.*; DestDir: {app}\modules\{#FFTW}\demos; Flags: recursesubdirs; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#FFTW}\includes; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\macros\lib; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\names; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.bin; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.sci; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.sce; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\tests\*.*; DestDir: {app}\modules\{#FFTW}\tests; Flags: recursesubdirs; Components: {#COMPN_FFTW} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
;
