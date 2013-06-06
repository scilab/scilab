;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) SCilab Enterprises - 2012 - Antoine ELIAS
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
; external_objects module
;--------------------------------------------------------------------------------------------------------------
;
#define EXTERNAL_OBJECTS "external_objects"
;
Source: bin\external_objects.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\external_objects.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#EXTERNAL_OBJECTS}\sci_gateway\{#EXTERNAL_OBJECTS}_gateway.xml; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#EXTERNAL_OBJECTS}\includes\*.h; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\includes\*.hxx; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#EXTERNAL_OBJECTS}\etc\{#EXTERNAL_OBJECTS}.quit; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\etc\{#EXTERNAL_OBJECTS}.start; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#EXTERNAL_OBJECTS}\macros\buildmacros.sce; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\buildmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\*.bin; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\*.sci; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\lib; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#EXTERNAL_OBJECTS}\macros\names; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#EXTERNAL_OBJECTS}\tests\*.*; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
