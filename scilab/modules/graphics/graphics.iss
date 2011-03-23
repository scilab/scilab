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
; graphics module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPHICS "graphics"
;
Source: modules\{#GRAPHICS}\VERSION.xml; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\license.txt; DestDir: {app}\modules\{#GRAPHICS}; Components: {#COMPN_GRAPHICS}
;
Source: thirdparty\jlatexmath-0.9.4.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\jlatexmath-fop-0.9.4.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
;
Source: bin\{#GRAPHICS}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;
Source: bin\no{#GRAPHICS}.dll; DestDir: {app}\bin; DestName: {#GRAPHICS}.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: modules\{#GRAPHICS}\sci_gateway\{#GRAPHICS}_gateway.xml; DestDir: {app}\modules\{#GRAPHICS}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.quit; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\{#GRAPHICS}.start; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\nvidia_bug_7575.sce; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\etc\intel_bug_7526.sce; DestDir: {app}\modules\{#GRAPHICS}\etc; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHICS}\includes\*.h; DestDir: {app}\modules\{#GRAPHICS}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\macros\lib; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\names; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.sci; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\*.bin; DestDir: {app}\modules\{#GRAPHICS}\macros; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHICS}\macros\datatips\lib; DestDir: {app}\modules\{#GRAPHICS}\macros\datatips; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHICS}\macros\datatips\names; DestDir: {app}\modules\{#GRAPHICS}\macros\datatips; Components: {#COMPN_GRAPHICS}
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
