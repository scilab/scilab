;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; cacsd module
;--------------------------------------------------------------------------------------------------------------
;
#define CACSD "cacsd"
;
Source: bin\slicot_f.dll; DestDir: {app}\bin; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\VERSION.xml; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\readme.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\license.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\changelog.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\sci_gateway\cacsd_gateway.xml; DestDir: {app}\modules\{#CACSD}\sci_gateway; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\etc\{#CACSD}.quit; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\etc\{#CACSD}.start; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\help\addchapter.sce; DestDir: {app}\modules\{#CACSD}\help; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
;
;Source: modules\{#CACSD}\includes\*.h; DestDir: {app}\modules\{#CACSD}\includes; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\macros\lib; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\names; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\*.bin; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\*.sci; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\buildmacros.sce; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\buildmacros.bat; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
Source: modules\{#CACSD}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\demos\*.*; DestDir: {app}\modules\{#CACSD}\demos; Flags: recursesubdirs; Components: {#COMPN_CACSD}
;
Source: modules\{#CACSD}\tests\*.*; DestDir: {app}\modules\{#CACSD}\tests; Flags: recursesubdirs; Components: {#COMPN_CACSD} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
