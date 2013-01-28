;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) Scilab Enterprises - 2012 - Antoine ELIAS
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
; uiwidget module
;--------------------------------------------------------------------------------------------------------------
;
#define UIWIDGET "uiwidget"
;
Source: modules\{#UIWIDGET}\license.txt; DestDir: {app}\modules\{#UIWIDGET}; Components: {#COMPN_SCILAB}
;
Source: modules\{#UIWIDGET}\jar\org.scilab.modules.{#UIWIDGET}.jar; DestDir: {app}\modules\{#UIWIDGET}\jar; Components: {#COMPN_SCILAB}
;
Source: modules\{#UIWIDGET}\sci_gateway\{#UIWIDGET}_gateway.xml; DestDir: {app}\modules\{#UIWIDGET}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#UIWIDGET}\etc\{#UIWIDGET}.quit; DestDir: {app}\modules\{#UIWIDGET}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#UIWIDGET}\etc\{#UIWIDGET}.start; DestDir: {app}\modules\{#UIWIDGET}\etc; Components: {#COMPN_SCILAB}
;
Source: bin\sciuiwidget.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\sciuiwidget.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#UIWIDGET}\macros\buildmacros.sce; DestDir: {app}\modules\{#UIWIDGET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UIWIDGET}\macros\buildmacros.bat; DestDir: {app}\modules\{#UIWIDGET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UIWIDGET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#UIWIDGET}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#UIWIDGET}\tests\*.*; DestDir: {app}\modules\{#UIWIDGET}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
