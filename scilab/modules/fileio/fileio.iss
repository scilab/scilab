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
; fileio module
;--------------------------------------------------------------------------------------------------------------
;
#define FILEIO "fileio"
;
Source: modules\{#FILEIO}\VERSION.xml; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\readme.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\license.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\changelog.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\sci_gateway\{#FILEIO}_gateway.xml; DestDir: {app}\modules\{#FILEIO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\etc\{#FILEIO}.quit; DestDir: {app}\modules\{#FILEIO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\etc\{#FILEIO}.start; DestDir: {app}\modules\{#FILEIO}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FILEIO}\includes\*.h; DestDir: {app}\modules\{#FILEIO}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\includes\getFullFilename.h; DestDir: {app}\modules\{#FILEIO}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\includes\dynlib_fileio.h; DestDir: {app}\modules\{#FILEIO}\includes; Components: {#COMPN_SCILAB}
;
Source: bin\fileio.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\lib; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\names; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.bin; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.sci; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FILEIO}\demos\*.*; DestDir: {app}\modules\{#FILEIO}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\tests\*.*; DestDir: {app}\modules\{#FILEIO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
