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

#ifndef SCILAB_X64
Filename: {app}\bin\SetupAtlas.exe; Description:{cm:DescriptionConfigureAtlas}; Flags: nowait; Components: {#COMPN_ATLAS_CPU_LIBRARY}
#endif
Filename: {code:getExecNameForDesktop}; Description: {cm:DescriptionLaunchScilab}; WorkingDir: "{userdocs}"; Flags: nowait postinstall skipifsilent;
;-------------------------------------------------------------------------------
;
