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
; cacsd module
;--------------------------------------------------------------------------------------------------------------
;
#define CACSD "cacsd"
;
Source: bin\slicot_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CACSD}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CACSD}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\license.txt; DestDir: {app}\modules\{#CACSD}; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\sci_gateway\{#CACSD}_gateway.xml; DestDir: {app}\modules\{#CACSD}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\etc\{#CACSD}.quit; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\etc\{#CACSD}.start; DestDir: {app}\modules\{#CACSD}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\macros\lib; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\macros\*.sci; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\macros\*.bin; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\macros\buildmacros.sce; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\macros\buildmacros.bat; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CACSD}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CACSD}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\demos\*.*; DestDir: {app}\modules\{#CACSD}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CACSD}\tests\*.*; DestDir: {app}\modules\{#CACSD}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
