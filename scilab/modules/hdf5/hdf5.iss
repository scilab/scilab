;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; hdf5 module
;--------------------------------------------------------------------------------------------------------------
;
#define HDF5 "hdf5"
;
Source: bin\{#HDF5}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\jar\org.scilab.modules.{#HDF5}.jar;DestDir: {app}\modules\{#HDF5}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: bin\nohdf5.dll; DestDir: {app}\bin; DestName: scihdf5.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#HDF5}\changelog.txt; DestDir: {app}\modules\{#HDF5}; Components: {#COMPN_SCILAB};
Source: modules\{#HDF5}\license.txt; DestDir: {app}\modules\{#HDF5}; Components: {#COMPN_SCILAB}
Source: modules\{#HDF5}\readme.txt; DestDir: {app}\modules\{#HDF5}; Components: {#COMPN_SCILAB}
Source: modules\{#HDF5}\version.xml; DestDir: {app}\modules\{#HDF5}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\sci_gateway\{#HDF5}_gateway.xml; DestDir: {app}\modules\{#HDF5}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HDF5}\includes\*.h; DestDir: {app}\modules\{#HDF5}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\etc\{#HDF5}.quit; DestDir: {app}\modules\{#HDF5}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HDF5}\etc\{#HDF5}.start; DestDir: {app}\modules\{#HDF5}\etc; Components: {#COMPN_SCILAB}
;
Source: thirdparty\j{#HDF5}.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#HDF5}\macros\buildmacros.sce; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\buildmacros.bat; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\*.bin; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\*.sci; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\lib; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HDF5}\macros\names; DestDir: {app}\modules\{#HDF5}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HDF5}\demos\*.*; DestDir: {app}\modules\{#HDF5}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HDF5}\tests\*.*; DestDir: {app}\modules\{#HDF5}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
