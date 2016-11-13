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
; hdf5 module
;--------------------------------------------------------------------------------------------------------------
;
#define HDF5 "hdf5"
;
Source: bin\{#HDF5}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: bin\nohdf5.dll; DestDir: {app}\bin; DestName: scihdf5.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#HDF5}\license.txt; DestDir: {app}\modules\{#HDF5}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\sci_gateway\{#HDF5}_gateway.xml; DestDir: {app}\modules\{#HDF5}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HDF5}\includes\*.h; DestDir: {app}\modules\{#HDF5}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\etc\{#HDF5}.quit; DestDir: {app}\modules\{#HDF5}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HDF5}\etc\{#HDF5}.start; DestDir: {app}\modules\{#HDF5}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HDF5}\macros\buildmacros.sce; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\buildmacros.bat; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\*.sci; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\lib; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HDF5}\demos\*.*; DestDir: {app}\modules\{#HDF5}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\tests\*.*; DestDir: {app}\modules\{#HDF5}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
