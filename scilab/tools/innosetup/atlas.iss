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
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------
; Atlas setup
;-------------------------------------------------------------------------------
;
; Librairie Atlas 3.8.0
Source: bin\atlas\*.dll; DestDir: {app}\bin\atlas; Components: {#COMPN_ATLAS_CPU_LIBRARY}
Source: bin\Atlas.spec; DestDir: {app}\bin; Components: {#COMPN_ATLAS_CPU_LIBRARY}
Source: bin\SetupAtlas.exe; DestDir: {app}\bin; Components: {#COMPN_ATLAS_CPU_LIBRARY}
;
;-------------------------------------------------------------------------------
;
