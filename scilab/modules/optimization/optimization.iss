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
; optimization module
;--------------------------------------------------------------------------------------------------------------
;
#define OPTIMIZATION "optimization"
;
Source: bin\{#OPTIMIZATION}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OPTIMIZATION}_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OPTIMIZATION}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OPTIMIZATION}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\license.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\sci_gateway\{#OPTIMIZATION}_gateway.xml; DestDir: {app}\modules\{#OPTIMIZATION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.quit; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.start; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\macros\lib; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\macros\*.sci; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#OPTIMIZATION}\macros\*.bin; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\demos\*.*; DestDir: {app}\modules\{#OPTIMIZATION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPTIMIZATION}\tests\*.*; DestDir: {app}\modules\{#OPTIMIZATION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB}  and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
