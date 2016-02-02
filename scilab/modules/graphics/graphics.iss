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
; graphics module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPHICS "graphics"
;
Source: modules\{#GRAPHICS}\license.txt; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
;
Source: thirdparty\jlatexmath-1.0.3.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\jlatexmath-fop-1.0.3.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
;
Source: bin\{#GRAPHICS}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\{#GRAPHICS}.lib; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\{#GRAPHICS}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;
Source: bin\no{#GRAPHICS}.dll; DestDir: {app}\bin; DestName: {#GRAPHICS}.dll; Components: not ({#COMPN_GRAPHICS})
Source: bin\no{#GRAPHICS}.lib; DestDir: {app}\bin; DestName: {#GRAPHICS}.lib; Components: not ({#COMPN_GRAPHICS})
Source: bin\no{#GRAPHICS}.dll; DestDir: {app}\bin; DestName: {#GRAPHICS}_gw.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: modules\{#GRAPHICS}\sci_gateway\{#GRAPHICS}_gateway.xml; DestDir: {app}\modules\{#GRAPHICS}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.quit; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.start; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\nvidia_bug_7575.sce; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHICS}\includes\*.h; DestDir: {app}\modules\{#GRAPHICS}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\lib; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.sci; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.bin; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\datatips\lib; DestDir: {app}\modules\{#GRAPHICS}\macros\datatips; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\datatips\*.sci; DestDir: {app}\modules\{#GRAPHICS}\macros\datatips; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\datatips\*.bin; DestDir: {app}\modules\{#GRAPHICS}\macros\datatips; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\tcl\*.*; DestDir: {app}\modules\{#GRAPHICS}\tcl; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\demos\*.*; DestDir: {app}\modules\{#GRAPHICS}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\tests\*.*; DestDir: {app}\modules\{#GRAPHICS}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
