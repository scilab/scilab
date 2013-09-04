;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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

Name: {commondesktop}\{#ScilabName}; Filename: {code:getExecNameForDesktop};WorkingDir: "{userdocs}";Comment: "Start Scilab - The open source platform for numerical computation"; MinVersion: 4,4; Tasks: desktopicon;
;
;shorcut -nwni for minimal install ( without JVM )
Name: {group}\{#ScilabName}; Filename: {code:getExecNameForDesktop};Parameters: "-nwni";WorkingDir: "{userdocs}";Components: not {#COMPN_JVM_MODULE};
;shortcut GUI + nwni for full install
Name: {group}\{#ScilabName}; Filename: {code:getExecNameForDesktop};WorkingDir: "{userdocs}";Components: {#COMPN_JVM_MODULE};
Name: {group}\{cm:ConsoleIconName}; Filename: {code:getExecNameForDesktop};Parameters: "-nw";WorkingDir: "{userdocs}";Components: {#COMPN_JVM_MODULE};
;
Name: {group}\{cm:DescriptionHelpScilabCHM}; Filename:"{app}\{cm:ScilabCHM}"; Components: {#COMPN_CHM};
;

Name: "{group}\{cm:WebIconScilab}"; Filename: "{app}\scilabwebsite.url";

Name: {group}\{cm:UnInstallIcon}; Filename: {uninstallexe};
;
;-------------------------------------------------------------------------------
;
