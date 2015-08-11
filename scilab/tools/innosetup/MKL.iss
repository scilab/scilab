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
; MKL library setup
;-------------------------------------------------------------------------------
;
Source: bin\mkl\blasplus.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY}
Source: bin\mkl\lapack.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY}
Source: bin\mkl\libifcoremd.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY}
Source: bin\mkl\libmmd.dll; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY}
Source: bin\mkl\readme-mkl.txt; DestDir: {app}\bin; Components: {#COMPN_MKL_CPU_LIBRARY}



