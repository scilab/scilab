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
; FFTW optimized by MKL library setup
;-------------------------------------------------------------------------------
;
Source: bin\mkl\libfftw3-3.dll; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\mkl\fftw-readme.txt; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW_MKL_LIBRARY}