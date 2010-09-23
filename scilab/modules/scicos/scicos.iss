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
; scicos module
;--------------------------------------------------------------------------------------------------------------
;
#define SCICOS "scicos"

Source: bin\no{#SCICOS}.lib; DestDir: {app}\bin; DestName: {#SCICOS}.lib; Components: not ({#COMPN_XCOS})
;
Source: bin\{#SCICOS}_f.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: bin\{#SCICOS}_sundials.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: bin\{#SCICOS}.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\{#SCICOS}.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\VERSION.xml; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\readme.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\license.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\changelog.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\sci_gateway\{#SCICOS}_gateway.xml; DestDir: {app}\modules\{#SCICOS}\sci_gateway; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\etc\{#SCICOS}.quit; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\etc\{#SCICOS}.start; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\palettes\*.cosf; DestDir: {app}\modules\{#SCICOS}\palettes; Flags: recursesubdirs; Components:  {#COMPN_XCOS}
;
;Source: modules\{#SCICOS}\includes\*.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\includes\scicos.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\includes\scicos_free.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\includes\scicos_malloc.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\includes\scicos_math.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\includes\scicos-def.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\lib; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\names; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS}\macros\*.cosf; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
;Source: modules\{#SCICOS}\demos\*.*; DestDir: {app}\modules\{#SCICOS}\demos; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS}\tests\*.*; DestDir: {app}\modules\{#SCICOS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
