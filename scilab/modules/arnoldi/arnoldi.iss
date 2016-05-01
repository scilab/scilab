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
; arnoldi module
;--------------------------------------------------------------------------------------------------------------
;
#define ARNOLDI "arnoldi"
;
Source: bin\{#ARNOLDI}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB};
;
Source: bin\Arpack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\license.txt; DestDir: {app}\modules\{#ARNOLDI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\sci_gateway\{#ARNOLDI}_gateway.xml; DestDir: {app}\modules\{#ARNOLDI}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.quit; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\etc\{#ARNOLDI}.start; DestDir: {app}\modules\{#ARNOLDI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\macros\*.sci; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\*.bin; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\buildmacros.sce; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\buildmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ARNOLDI}\macros\lib; DestDir: {app}\modules\{#ARNOLDI}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#ARNOLDI}\tests\*.*; DestDir: {app}\modules\{#ARNOLDI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
