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
; parallel module
;--------------------------------------------------------------------------------------------------------------
;
#define PARALLEL "parallel"
;
;
Source: modules\{#PARALLEL}\license.txt; DestDir: {app}\modules\{#PARALLEL}; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARALLEL}\sci_gateway\{#PARALLEL}_gateway.xml; DestDir: {app}\modules\{#PARALLEL}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARALLEL}\etc\{#PARALLEL}.quit; DestDir: {app}\modules\{#PARALLEL}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PARALLEL}\etc\{#PARALLEL}.start; DestDir: {app}\modules\{#PARALLEL}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARALLEL}\macros\buildmacros.sce; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PARALLEL}\macros\buildmacros.bat; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PARALLEL}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARALLEL}\tests\*.*; DestDir: {app}\modules\{#PARALLEL}\tests; Flags: recursesubdirs; Components: {#COMPN_TESTS} and {#COMPN_SCILAB}
#endif
;--------------------------------------------------------------------------------------------------------------
