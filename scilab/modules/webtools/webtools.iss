;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; sound module
;--------------------------------------------------------------------------------------------------------------
;
#define WEBTOOLS "webtools"
;
Source: bin\{#WEBTOOLS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#WEBTOOLS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\license.txt; DestDir: {app}\modules\{#WEBTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\sci_gateway\{#WEBTOOLS}_gateway.xml; DestDir: {app}\modules\{#WEBTOOLS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\etc\{#WEBTOOLS}.quit; DestDir: {app}\modules\{#WEBTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\etc\{#WEBTOOLS}.start; DestDir: {app}\modules\{#WEBTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\*.sci; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WEBTOOLS}\macros\*.bin; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WEBTOOLS}\macros\lib; DestDir: {app}\modules\{#WEBTOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#WEBTOOLS}\tests\*.*; DestDir: {app}\modules\{#WEBTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
