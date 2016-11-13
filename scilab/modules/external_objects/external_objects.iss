;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) SCilab Enterprises - 2012 - Antoine ELIAS
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
; external_objects module
;--------------------------------------------------------------------------------------------------------------
;
#define EXTERNAL_OBJECTS "external_objects"
;
Source: bin\{#EXTERNAL_OBJECTS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#EXTERNAL_OBJECTS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
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
;Source: modules\{#EXTERNAL_OBJECTS}\macros\names; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#EXTERNAL_OBJECTS}\tests\*.*; DestDir: {app}\modules\{#EXTERNAL_OBJECTS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
