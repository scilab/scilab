;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
;Source: modules\{#COMPLETION}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\*.bin; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\*.sci; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\lib; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\names; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COMPLETION}\demos\*.*; DestDir: {app}\modules\{#COMPLETION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\tests\*.*; DestDir: {app}\modules\{#COMPLETION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
