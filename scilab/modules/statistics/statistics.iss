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
; statistics module
;--------------------------------------------------------------------------------------------------------------
;
#define STATISTICS "statistics"

Source: bin\{#STATISTICS}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#STATISTICS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\dcd_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#STATISTICS}\license.txt; DestDir: {app}\modules\{#STATISTICS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#STATISTICS}\sci_gateway\{#STATISTICS}_gateway.xml; DestDir: {app}\modules\{#STATISTICS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#STATISTICS}\etc\{#STATISTICS}.quit; DestDir: {app}\modules\{#STATISTICS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\etc\{#STATISTICS}.start; DestDir: {app}\modules\{#STATISTICS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#STATISTICS}\macros\lib; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\macros\*.sci; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\macros\*.bin; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\macros\buildmacros.sce; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\macros\buildmacros.bat; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STATISTICS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#STATISTICS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#STATISTICS}\tests\*.*; DestDir: {app}\modules\{#STATISTICS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
