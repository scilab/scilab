;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
Source: bin\{#SCICOS_BLOCKS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: bin\{#SCICOS_BLOCKS}.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS_BLOCKS}_f.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\VERSION.xml; DestDir: {app}\modules\{#SCICOS_BLOCKS}; Components: {#COMPN_XCOS}
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
Source: modules\{#SCICOS_BLOCKS}\macros\names; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.mo; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\blocks\*.h5; DestDir: {app}\modules\{#SCICOS_BLOCKS}\blocks; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\src\scripts\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\src\scripts; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\tests\*.*; DestDir: {app}\modules\{#SCICOS_BLOCKS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
