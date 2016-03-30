;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) Scilab Enterprises - 2016 - Antoine ELIAS
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
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; completion module
;--------------------------------------------------------------------------------------------------------------
;
#define NODE "node"
; batch files
Source: {#NODE}\linux-launcher.sh; DestDir: {app}\{#NODE}; Components: {#COMPN_SCILAB}
Source: {#NODE}\windows-launcher.bat; DestDir: {app}\{#NODE}; Components: {#COMPN_SCILAB}

; server files
Source: {#NODE}\scilab_server.js; DestDir: {app}\{#NODE}; Components: {#COMPN_SCILAB}
Source: {#NODE}\scilab_process.js; DestDir: {app}\{#NODE}; Components: {#COMPN_SCILAB}

; start script
Source: {#NODE}\start.sce; DestDir: {app}\{#NODE}; Components: {#COMPN_SCILAB}

;ressources files
Source: {#NODE}\static\*.*; DestDir: {app}\{#NODE}\static; Flags: recursesubdirs; Components: {#COMPN_SCILAB}

;node js extentions socket.io, express socket.io-client
Source: {#NODE}\node_modules\*.*; DestDir: {app}\{#NODE}\node_modules; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
