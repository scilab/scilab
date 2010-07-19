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
; pvm module
;--------------------------------------------------------------------------------------------------------------
;
#define PVM "pvm"
;
;Source: bin\{#PVM}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: modules\{#PVM}\changelog.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\license.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\readme.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\VERSION.xml; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\includes\*.h; DestDir: {app}\modules\{#PVM}\includes; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\sci_gateway\{#PVM}_gateway.xml; DestDir: {app}\modules\{#PVM}\sci_gateway; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\etc\{#PVM}.quit; DestDir: {app}\modules\{#PVM}\etc; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\etc\{#PVM}.start; DestDir: {app}\modules\{#PVM}\etc; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\macros\buildmacros.sce; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\buildmacros.bat; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\*.bin; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\*.sci; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\lib; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\names; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\demos\*.*; DestDir: {app}\modules\{#PVM}\demos; Flags: recursesubdirs; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\src\pvm3\Readme.Win32; DestDir: {app}\modules\{#PVM}\src\pvm3; Flags: ignoreversion; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\bin\WIN32\hoster.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\bin\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\bin\WIN32\pvmgs.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\bin\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\include\*.h; DestDir: {app}\modules\{#PVM}\src\pvm3\include; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\pvm.bat; DestDir: {app}\modules\{#PVM}\src\pvm3\lib; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\pvmd.bat; DestDir: {app}\modules\{#PVM}\src\pvm3\lib; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.lib; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.dll; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\pvmconf.bat; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\pvmconf.sce; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\tests\*.*; DestDir: {app}\modules\{#PVM}\tests; Flags: recursesubdirs; Components: {#COMPN_PVM} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
