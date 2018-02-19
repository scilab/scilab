;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
; Copyright (C) ESI - 2018 - Antoine ELIAS
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

;no java
Name: {commondesktop}\{#ScilabName}{cm:NWNIIconName}; Filename: {app}\bin\Scilex.exe;WorkingDir: "%USERPROFILE%\Documents";Comment: "Start Scilab - The open source platform for numerical computation"; MinVersion: 4,4; Tasks: desktopicon;Components: not {#COMPN_JVM_MODULE};
Name: {group}\{#ScilabName}{cm:NWNIIconName}; Filename: {app}\bin\Scilex.exe;WorkingDir: "%USERPROFILE%\Documents";Components: not {#COMPN_JVM_MODULE};

;java
;desktop
Name: {commondesktop}\{#ScilabName}{cm:GUIIconName}; Filename: {app}\bin\WScilex.exe;WorkingDir: "%USERPROFILE%\Documents";Comment: "Start Scilab - The open source platform for numerical computation"; MinVersion: 4,4; Tasks: desktopicon;Components: {#COMPN_JVM_MODULE};
;start menu
Name: {group}\{#ScilabName}{cm:GUIIconName}; Filename: {app}\bin\WScilex.exe;WorkingDir: "%USERPROFILE%\Documents";Components: {#COMPN_JVM_MODULE};
Name: {group}\{#ScilabName}{cm:NWIconName}; Filename: {app}\bin\WScilex-cli.exe;WorkingDir: "%USERPROFILE%\Documents";Components: {#COMPN_JVM_MODULE};
Name: {group}\{#ScilabName}{cm:NWNIIconName}; Filename: {app}\bin\Scilex.exe;WorkingDir: "%USERPROFILE%\Documents";Components: {#COMPN_JVM_MODULE};

;
Name: "{group}\{cm:WebIconScilab}"; Filename: "{app}\scilabwebsite.url";

Name: {group}\{cm:UnInstallIcon}; Filename: {uninstallexe};
;
;-------------------------------------------------------------------------------
;
