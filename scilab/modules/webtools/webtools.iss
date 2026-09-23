;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) ESI - 2017 - Antoine ELIAS
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
; webtools module
;--------------------------------------------------------------------------------------------------------------
;
#define WEBTOOLS "webtools"
;
Source: bin\{#WEBTOOLS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#WEBTOOLS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#WEBTOOLS}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#WEBTOOLS}_gw.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\license.txt; DestDir: {app}\modules\{#WEBTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\etc\{#WEBTOOLS}.quit; DestDir: {app}\modules\{#WEBTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\etc\{#WEBTOOLS}.start; DestDir: {app}\modules\{#WEBTOOLS}\etc; Components: {#COMPN_SCILAB}
;
; Stylesheets rendering the inline help in the LSP hover and signature help.
Source: modules\{#WEBTOOLS}\etc\*.xsl; DestDir: {app}\modules\{#WEBTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\includes\*.h; DestDir: {app}\modules\{#WEBTOOLS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\includes\*.hxx; DestDir: {app}\modules\{#WEBTOOLS}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WEBTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\*.sci; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\*.bin; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\lib; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\demos\*.*; DestDir: {app}\modules\{#WEBTOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\tests\*.*; DestDir: {app}\modules\{#WEBTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
Source: modules\{#WEBTOOLS}\help\*.*; DestDir: {app}\modules\{#WEBTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
