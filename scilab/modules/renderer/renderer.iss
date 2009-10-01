;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007 - INRIA - Allan CORNET
; Version TRUNK
; Script d'installation Inno Setup (5.2.1) pour scilab
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; renderer module
;--------------------------------------------------------------------------------------------------------------
;
#define RENDERER "renderer"
;
Source: bin\jogl.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\jogl_awt.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\jogl_cg.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}

Source: bin\scirenderer.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;Source: bin\norenderer.dll; DestDir: {app}\bin; DestName: scirenderer.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: thirdparty\gluegen-rt.jar; DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\jogl.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
;
Source: thirdparty\fonts\scilabsymbols.ttf;DestDir: {app}\thirdparty\fonts; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\jar\org.scilab.modules.renderer.jar;DestDir: {app}\modules\{#RENDERER}\jar; Components: {#COMPN_GRAPHICS}
;

Source: modules\{#RENDERER}\changelog.txt; DestDir: {app}\modules\{#RENDERER}; Components: {#COMPN_GRAPHICS};
Source: modules\{#RENDERER}\license.txt; DestDir: {app}\modules\{#RENDERER}; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\readme.txt; DestDir: {app}\modules\{#RENDERER}; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\version.xml; DestDir: {app}\modules\{#RENDERER}; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\sci_gateway\renderer_gateway.xml; DestDir: {app}\modules\{#RENDERER}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\includes\*.h; DestDir: {app}\modules\{#RENDERER}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\help\addchapter.sce; DestDir: {app}\modules\{#RENDERER}\help; Flags: recursesubdirs; Components:  {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\etc\{#RENDERER}.quit; DestDir: {app}\modules\{#RENDERER}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\etc\{#RENDERER}.start; DestDir: {app}\modules\{#RENDERER}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\macros\buildmacros.sce; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\macros\buildmacros.bat; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\*.bin; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\*.sci; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\lib; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\names; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\demos\*.*; DestDir: {app}\modules\{#RENDERER}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\tests\*.*; DestDir: {app}\modules\{#RENDERER}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
