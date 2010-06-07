;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - 2008 - Allan CORNET
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
; graphic_export module
;--------------------------------------------------------------------------------------------------------------
;
;
#define GRAPHIC_EXPORT "graphic_export"
;
Source: bin\sci{#GRAPHIC_EXPORT}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\no{#GRAPHIC_EXPORT}.dll; DestDir: {app}\bin; DestName: sci{#GRAPHIC_EXPORT}.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: tools\pstoedit\*.*; DestDir: {app}\tools\pstoedit; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS};
;
Source: modules\{#GRAPHIC_EXPORT}\jar\org.scilab.modules.{#GRAPHIC_EXPORT}.jar;DestDir: {app}\modules\{#GRAPHIC_EXPORT}\jar; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\changelog.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS};
Source: modules\{#GRAPHIC_EXPORT}\license.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\readme.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\version.xml; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\sci_gateway\{#GRAPHIC_EXPORT}_gateway.xml; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHIC_EXPORT}\includes\*.h; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\etc\{#GRAPHIC_EXPORT}.quit; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\etc\{#GRAPHIC_EXPORT}.start; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\*.bin; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\*.sci; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\lib; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#GRAPHIC_EXPORT}\macros\names; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\macros; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#GRAPHIC_EXPORT}\demos\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\tests\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
