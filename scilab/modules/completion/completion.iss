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
; completion module
;--------------------------------------------------------------------------------------------------------------
;
#define COMPLETION "completion"
;
Source: bin\sci{#COMPLETION}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\sci{#COMPLETION}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\jar\org.scilab.modules.{#COMPLETION}.jar;DestDir: {app}\modules\{#COMPLETION}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: bin\nosci{#COMPLETION}.dll; DestDir: {app}\bin; DestName: sci{#COMPLETION}.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#COMPLETION}\license.txt; DestDir: {app}\modules\{#COMPLETION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\sci_gateway\{#COMPLETION}_gateway.xml; DestDir: {app}\modules\{#COMPLETION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\includes\*.h; DestDir: {app}\modules\{#COMPLETION}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\etc\{#COMPLETION}.quit; DestDir: {app}\modules\{#COMPLETION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\etc\{#COMPLETION}.start; DestDir: {app}\modules\{#COMPLETION}\etc; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#COMPLETION}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COMPLETION}\demos\*.*; DestDir: {app}\modules\{#COMPLETION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\tests\*.*; DestDir: {app}\modules\{#COMPLETION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
