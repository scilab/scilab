;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; FFTW module
;--------------------------------------------------------------------------------------------------------------
;
#define FFTW "fftw"
;
Source: bin\{#FFTW}\libfftw3-3.dll; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW} and not {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\{#FFTW}\COPYRIGHT; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW} and not {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\{#FFTW}\COPYING; DestDir: {app}\bin\{#FFTW}; Components: {#COMPN_FFTW} and not {#COMPN_FFTW_MKL_LIBRARY}

Source: bin\{#FFTW}.dll; DestDir: {app}\bin; DestName: {#FFTW}.dll; Components: {#COMPN_FFTW}
Source: bin\no{#FFTW}.dll; DestDir: {app}\bin; DestName: {#FFTW}.dll; Components: not ({#COMPN_FFTW})
;

Source: modules\{#FFTW}\license.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\etc\{#FFTW}.quit; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\etc\{#FFTW}.start; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\demos\*.*; DestDir: {app}\modules\{#FFTW}\demos; Flags: recursesubdirs; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#FFTW}\includes; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\macros\lib; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.sci; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.sce; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\tests\*.*; DestDir: {app}\modules\{#FFTW}\tests; Flags: recursesubdirs; Components: {#COMPN_FFTW} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
;
