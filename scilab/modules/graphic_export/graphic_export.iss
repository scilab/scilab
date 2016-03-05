;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - 2008 - Allan CORNET
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
; graphic_export module
;--------------------------------------------------------------------------------------------------------------
;
;
#define GRAPHIC_EXPORT "graphic_export"
;
Source: bin\sci{#GRAPHIC_EXPORT}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\no{#GRAPHIC_EXPORT}.dll; DestDir: {app}\bin; DestName: sci{#GRAPHIC_EXPORT}.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: thirdparty\freehep-io.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\freehep-util.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\freehep-graphics2d.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\freehep-graphicsio-emf.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\freehep-graphicsio.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\jar\org.scilab.modules.{#GRAPHIC_EXPORT}.jar;DestDir: {app}\modules\{#GRAPHIC_EXPORT}\jar; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\license.txt; DestDir: {app}\modules\{#GRAPHIC_EXPORT}; Components: {#COMPN_GRAPHICS}
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
;
;Source: modules\{#GRAPHIC_EXPORT}\demos\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#GRAPHIC_EXPORT}\tests\*.*; DestDir: {app}\modules\{#GRAPHIC_EXPORT}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
