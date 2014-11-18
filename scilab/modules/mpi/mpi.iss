;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) Scilab Enterprises - Antoine ELIAS
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; MPI module
;--------------------------------------------------------------------------------------------------------------
;
#define MPILIB "mpi"
;
Source: bin\{#MPILIB}.dll; DestDir: {app}\bin; Components: {#COMPN_MPI}
Source: bin\{#MPILIB}.lib; DestDir: {app}\bin; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\etc\{#MPILIB}.quit; DestDir: {app}\modules\{#MPILIB}\etc; Components: {#COMPN_MPI}
Source: modules\{#MPILIB}\etc\{#MPILIB}.start; DestDir: {app}\modules\{#MPILIB}\etc; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\includes\*.h; DestDir: {app}\modules\{#MPILIB}\includes; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\sci_gateway\{#MPILIB}_gateway.xml; DestDir: {app}\modules\{#MPILIB}\sci_gateway; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MPILIB}\macros; Components: {#COMPN_MPI}
Source: modules\{#MPILIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MPILIB}\macros; Components: {#COMPN_MPI}
Source: modules\{#MPILIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MPILIB}\macros; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\examples\*.*; DestDir: {app}\modules\{#MPILIB}\examples; Flags: recursesubdirs; Components: {#COMPN_MPI}
;
Source: modules\{#MPILIB}\tests\*.*; DestDir: {app}\modules\{#MPILIB}\tests; Flags: recursesubdirs; Components: {#COMPN_MPI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
