;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2011 - Allan CORNET
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
; threads module
;--------------------------------------------------------------------------------------------------------------
;
#define THREADS "threads"
;
Source: bin\{#THREADS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#THREADS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#THREADS}\includes\dynlib_threads.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#THREADS}\includes\Thread_Wrapper.h; DestDir: {app}\modules\{#THREADS}\includes; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
