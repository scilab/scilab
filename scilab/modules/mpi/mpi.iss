;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) Scilab Enterprises - Antoine ELIAS
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
