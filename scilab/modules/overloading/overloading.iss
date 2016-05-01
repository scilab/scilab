;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; overloading module
;--------------------------------------------------------------------------------------------------------------
;
#define OVERLOADING "overloading"
;
Source: modules\{#OVERLOADING}\license.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.quit; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.start; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\includes\*.h; DestDir: {app}\modules\{#OVERLOADING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\macros\buildmacros.sce; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\buildmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\lib; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.sci; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.bin; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\demos\*.*; DestDir: {app}\modules\{#OVERLOADING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\tests\*.*; DestDir: {app}\modules\{#OVERLOADING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
