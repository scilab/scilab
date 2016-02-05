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
; data_structures module
;--------------------------------------------------------------------------------------------------------------
;
#define DATASTRUCT "data_structures"
;
Source: bin\{#DATASTRUCT}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\license.txt; DestDir: {app}\modules\{#DATASTRUCT}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\sci_gateway\{#DATASTRUCT}_gateway.xml; DestDir: {app}\modules\{#DATASTRUCT}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\etc\{#DATASTRUCT}.quit; DestDir: {app}\modules\{#DATASTRUCT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\etc\{#DATASTRUCT}.start; DestDir: {app}\modules\{#DATASTRUCT}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DATASTRUCT}\includes\*.h; DestDir: {app}\modules\{#DATASTRUCT}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\macros\lib; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\*.sci; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\*.bin; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\buildmacros.sce; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\buildmacros.bat; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DATASTRUCT}\demos\*.*; DestDir: {app}\modules\{#DATASTRUCT}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\tests\*.*; DestDir: {app}\modules\{#DATASTRUCT}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
