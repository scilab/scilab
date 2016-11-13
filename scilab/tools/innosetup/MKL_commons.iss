;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2015 - Scilab Enterprises
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
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------
; MKL common files (used by MKL BLAS Lapack and MKL FFTW)
;-------------------------------------------------------------------------------
;
Source: bin\mkl\mkl_core.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\mkl\mkl_intel_thread.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\mkl\libiomp5md.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY}