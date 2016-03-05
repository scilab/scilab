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
; FFTW optimized by MKL library setup
;-------------------------------------------------------------------------------
;
Source: bin\mkl\libfftw3-3.dll; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW_MKL_LIBRARY}
Source: bin\mkl\fftw-readme.txt; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW_MKL_LIBRARY}