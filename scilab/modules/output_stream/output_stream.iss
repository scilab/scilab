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
; output_stream module
;--------------------------------------------------------------------------------------------------------------
;
#define OUTPUT_STREAM "output_stream"
;
Source: bin\{#OUTPUT_STREAM}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OUTPUT_STREAM}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OUTPUT_STREAM}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\license.txt; DestDir: {app}\modules\{#OUTPUT_STREAM}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OUTPUT_STREAM}\includes\*.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\includes\Scierror.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\includes\sciprint.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\includes\do_error_number.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\includes\cerro.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\includes\dynlib_output_stream.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\sci_gateway\{#OUTPUT_STREAM}_gateway.xml; DestDir: {app}\modules\{#OUTPUT_STREAM}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\etc\{#OUTPUT_STREAM}.quit; DestDir: {app}\modules\{#OUTPUT_STREAM}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\etc\{#OUTPUT_STREAM}.start; DestDir: {app}\modules\{#OUTPUT_STREAM}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\macros\buildmacros.sce; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\buildmacros.bat; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\*.bin; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\*.sci; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\lib; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\names; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OUTPUT_STREAM}\demos\*.*; DestDir: {app}\modules\{#OUTPUT_STREAM}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\tests\*.*; DestDir: {app}\modules\{#OUTPUT_STREAM}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
