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
; signal_processing module
;--------------------------------------------------------------------------------------------------------------
;
#define SIGPROC "signal_processing"
;
Source: bin\{#SIGPROC}_f.dll; DestDir: {app}\bin; Components:  {#COMPN_SCILAB}
Source: bin\{#SIGPROC}.dll; DestDir: {app}\bin; Components:  {#COMPN_SCILAB}
Source: bin\{#SIGPROC}_gw.dll; DestDir: {app}\bin; Components:  {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\license.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\sci_gateway\{#SIGPROC}_gateway.xml; DestDir: {app}\modules\{#SIGPROC}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.quit; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.start; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\macros\lib; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\macros\*.sci; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\macros\*.bin; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SIGPROC}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\demos\*.*; DestDir: {app}\modules\{#SIGPROC}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIGPROC}\tests\*.*; DestDir: {app}\modules\{#SIGPROC}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
