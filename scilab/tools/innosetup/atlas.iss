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
