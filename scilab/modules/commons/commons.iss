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
; commons module
;--------------------------------------------------------------------------------------------------------------
;
#define COMMONS "commons"
;
Source: modules\{#COMMONS}\jar\org.scilab.modules.{#COMMONS}.jar;DestDir: {app}\modules\{#COMMONS}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: bin\sci{#COMMONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\license.txt; DestDir: {app}\modules\{#COMMONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\etc\{#COMMONS}.quit; DestDir: {app}\modules\{#COMMONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\etc\{#COMMONS}.start; DestDir: {app}\modules\{#COMMONS}\etc; Components: {#COMPN_SCILAB}
;Source: modules\{#COMMONS}\etc\*.xsl; DestDir: {app}\modules\{#COMMONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COMMONS}\tests\*.*; DestDir: {app}\modules\{#COMMONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
