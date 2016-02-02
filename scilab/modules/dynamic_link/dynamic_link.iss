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
; dynamic_link module
;--------------------------------------------------------------------------------------------------------------
;
#define DYNAMIC_LINK "dynamic_link"
;
Source: bin\{#DYNAMIC_LINK}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#DYNAMIC_LINK}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#DYNAMIC_LINK}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;
Source: modules\{#DYNAMIC_LINK}\license.txt; DestDir: {app}\modules\{#DYNAMIC_LINK}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\etc\{#DYNAMIC_LINK}.quit; DestDir: {app}\modules\{#DYNAMIC_LINK}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\etc\{#DYNAMIC_LINK}.start; DestDir: {app}\modules\{#DYNAMIC_LINK}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\sci_gateway\{#DYNAMIC_LINK}_gateway.xml; DestDir: {app}\modules\{#DYNAMIC_LINK}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\src\scripts\Makedll.incl; DestDir: {app}\modules\{#DYNAMIC_LINK}\src\scripts; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\src\scripts\TEMPLATE_MAKEFILE.VC; DestDir: {app}\modules\{#DYNAMIC_LINK}\src\scripts; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\src\scripts\Makefile.incl.mak; DestDir: {app}\modules\{#DYNAMIC_LINK}\src\scripts; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\includes\*.h; DestDir: {app}\modules\{#DYNAMIC_LINK}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\macros\lib; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\*.sci; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\*.bin; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\buildmacros.sce; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\buildmacros.bat; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\demos\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\examples\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\tests\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
