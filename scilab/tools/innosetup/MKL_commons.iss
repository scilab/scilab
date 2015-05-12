;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2015 - Scilab Enterprises
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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