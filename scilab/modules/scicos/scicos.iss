;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
; Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; action_binding module
;--------------------------------------------------------------------------------------------------------------
;
#define SCICOS "scicos"

Source: bin\noscicos.lib; DestDir: {app}\bin; DestName: scicos.lib; Components: not ({#COMPN_SCICOS})
Source: bin\noscicos.dll; DestDir: {app}\bin; DestName: scicos.dll; Components: not ({#COMPN_SCICOS})
;
Source: bin\scicos_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCICOS}
Source: bin\scicos_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCICOS}
;
Source: bin\scicos_sundials.dll; DestDir: {app}\bin; Components: {#COMPN_SCICOS}
;
Source: bin\scicos.lib; DestDir: {app}\bin; Components: {#COMPN_SCICOS}
Source: bin\scicos.dll; DestDir: {app}\bin; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\VERSION.xml; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\readme.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\license.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\changelog.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\sci_gateway\scicos_gateway.xml; DestDir: {app}\modules\{#SCICOS}\sci_gateway; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\etc\{#SCICOS}.quit; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\etc\{#SCICOS}.start; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_SCICOS}
;
;
Source: modules\{#SCICOS}\help\addchapter.sce; DestDir: {app}\modules\{#SCICOS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCICOS}
Source: modules\{#SCICOS}\help\scicos_help.jar; DestDir: {app}\modules\{#SCICOS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\includes\*.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\lib; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\names; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
Source: modules\{#SCICOS}\macros\*.cosf; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\demos\*.*; DestDir: {app}\modules\{#SCICOS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
;
;Source: modules\{#SCICOS}\tcl\*.*; DestDir: {app}\modules\{#SCICOS}\bin; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
;Source: bin\.scicos_pal; DestDir: {app}\bin; Flags: ignoreversion; Components: {#COMPN_SCICOS}
;
Source: modules\{#SCICOS}\tests\*.*; DestDir: {app}\modules\{#SCICOS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCICOS} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
