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
; optimization module
;--------------------------------------------------------------------------------------------------------------
;
#define OPTIMIZATION "optimization"
;
Source: bin\{#OPTIMIZATION}_f.dll; DestDir: {app}\bin; Components: {#COMPN_OPTIM}
Source: bin\{#OPTIMIZATION}_f.lib; DestDir: {app}\bin; Components: {#COMPN_OPTIM}
Source: bin\{#OPTIMIZATION}.dll; DestDir: {app}\bin; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\VERSION.xml; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\readme.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\license.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\changelog.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\sci_gateway\{#OPTIMIZATION}_gateway.xml; DestDir: {app}\modules\{#OPTIMIZATION}\sci_gateway; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.quit; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.start; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_OPTIM}
;
;Source: modules\{#OPTIMIZATION}\includes\*.h; DestDir: {app}\modules\{#OPTIMIZATION}\includes; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\lib; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\names; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\*.bin; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\*.sci; DestDir: {app}\modules\{#OPTIMIZATION}\macros;Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\demos\*.*; DestDir: {app}\modules\{#OPTIMIZATION}\demos; Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\tests\*.*; DestDir: {app}\modules\{#OPTIMIZATION}\tests; Flags: recursesubdirs; Components: {#COMPN_OPTIM}  and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
