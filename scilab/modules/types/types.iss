;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; types module
;--------------------------------------------------------------------------------------------------------------
;
#define TYPES "types"
;
Source: modules\{#TYPES}\jar\org.scilab.modules.{#TYPES}.jar;DestDir: {app}\modules\{#TYPES}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#TYPES}\license.txt; DestDir: {app}\modules\{#TYPES}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#TYPES}\etc\{#TYPES}.quit; DestDir: {app}\modules\{#TYPES}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#TYPES}\etc\{#TYPES}.start; DestDir: {app}\modules\{#TYPES}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#TYPES}\macros\buildmacros.sce; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#TYPES}\macros\buildmacros.bat; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#TYPES}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#TYPES}\macros\lib; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#TYPES}\tests\*.*; DestDir: {app}\modules\{#TYPES}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
