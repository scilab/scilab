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

#ifndef SCILAB_X64
Filename: {app}\bin\SetupAtlas.exe; Description:{cm:DescriptionConfigureAtlas}; Flags: nowait; Components: {#COMPN_ATLAS_CPU_LIBRARY}
#endif
Filename: {code:getExecNameForDesktop}; Description: {cm:DescriptionLaunchScilab}; WorkingDir: "{userdocs}"; Flags: nowait postinstall skipifsilent;Components: {#COMPN_JVM_MODULE}
Filename: {code:getExecNameForDesktop};Parameters: "-nwni"; Description: {cm:DescriptionLaunchScilab}; WorkingDir: "{userdocs}"; Flags: nowait postinstall skipifsilent;Components: not {#COMPN_JVM_MODULE}
;-------------------------------------------------------------------------------
;
