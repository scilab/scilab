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
; statistics module
;--------------------------------------------------------------------------------------------------------------
;
#define STATISTICS "statistics"

Source: bin\{#STATISTICS}.dll; DestDir: {app}\bin; Components: {#COMPN_STATS}
Source: bin\dcd_f.dll; DestDir: {app}\bin; Components: {#COMPN_STATS}
;
Source: modules\{#STATISTICS}\VERSION.xml; DestDir: {app}\modules\{#STATISTICS}; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\readme.txt; DestDir: {app}\modules\{#STATISTICS}; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\license.txt; DestDir: {app}\modules\{#STATISTICS}; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\changelog.txt; DestDir: {app}\modules\{#STATISTICS}; Components: {#COMPN_STATS}
;
Source: modules\{#STATISTICS}\sci_gateway\{#STATISTICS}_gateway.xml; DestDir: {app}\modules\{#STATISTICS}\sci_gateway; Components: {#COMPN_STATS}
;
Source: modules\{#STATISTICS}\etc\{#STATISTICS}.quit; DestDir: {app}\modules\{#STATISTICS}\etc; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\etc\{#STATISTICS}.start; DestDir: {app}\modules\{#STATISTICS}\etc; Components: {#COMPN_STATS}
;
Source: modules\{#STATISTICS}\macros\lib; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\names; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\*.bin; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\*.sci; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\buildmacros.sce; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\buildmacros.bat; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATISTICS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_STATS}
;
;Source: modules\{#STATISTICS}\demos\*.*; DestDir: {app}\modules\{#STATISTICS}\demos; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
Source: modules\{#STATISTICS}\tests\*.*; DestDir: {app}\modules\{#STATISTICS}\tests; Flags: recursesubdirs; Components: {#COMPN_STATS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
