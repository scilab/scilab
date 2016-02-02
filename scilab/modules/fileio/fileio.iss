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
; fileio module
;--------------------------------------------------------------------------------------------------------------
;
#define FILEIO "fileio"
;
Source: bin\{#FILEIO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FILEIO}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FILEIO}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\license.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
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
Source: modules\{#FILEIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\lib; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.sci; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.bin; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FILEIO}\demos\*.*; DestDir: {app}\modules\{#FILEIO}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\tests\*.*; DestDir: {app}\modules\{#FILEIO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
