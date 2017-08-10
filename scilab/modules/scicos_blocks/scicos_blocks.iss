;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
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
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; scicos_blocks module
;--------------------------------------------------------------------------------------------------------------
;
#define SCICOS_BLOCKS "scicos_blocks"
;--------------------------------------------------------------------------------------------------------------
;
Source: bin\{#SCICOS_BLOCKS}.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}-cli.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}-cli_f.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: bin\{#SCICOS_BLOCKS}.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}-cli.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}_f.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}-cli_f.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\license.txt; DestDir: {app}\modules\{#SCICOS_BLOCKS}; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\etc\{#SCICOS_BLOCKS}.quit; DestDir: {app}\modules\{#SCICOS_BLOCKS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\etc\{#SCICOS_BLOCKS}.start; DestDir: {app}\modules\{#SCICOS_BLOCKS}\etc; Components: {#COMPN_XCOS}
;
;Source: modules\{#SCICOS_BLOCKS}\includes\*.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\blocks.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\scicos_block.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\scicos_block4.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
;
Source: bin\modelicac.exe; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\modelicat.exe; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\xml2modelica.exe; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\macros\lib; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.mo; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\src\scripts\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\src\scripts; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\tests\*.*; DestDir: {app}\modules\{#SCICOS_BLOCKS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
