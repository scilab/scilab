;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab

; Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
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
; xcos module
;--------------------------------------------------------------------------------------------------------------
;
#define XCOS "xcos"
;
Source: bin\sci{#XCOS}.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\jar\org.scilab.modules.{#XCOS}.jar; DestDir: {app}\modules\{#XCOS}\jar; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\license.txt; DestDir: {app}\modules\{#XCOS}; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\sci_gateway\{#XCOS}_gateway.xml; DestDir: {app}\modules\{#XCOS}\sci_gateway; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\etc\{#XCOS}.quit; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\{#XCOS}.start; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\*.xml; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\*.xsl; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\*.xsd; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\includes\*.h; DestDir: {app}\modules\{#XCOS}\includes; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\images\icons\*.png; DestDir: {app}\modules\{#XCOS}\images\icons; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\images\blocks\*.svg; DestDir: {app}\modules\{#XCOS}\images\blocks; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\images\gui\*.gif; DestDir: {app}\modules\{#XCOS}\images\gui; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\images\palettes\*.png; DestDir: {app}\modules\{#XCOS}\images\palettes; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\macros\buildmacros.sce; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\buildmacros.bat; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\lib; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\*.sci; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\*.bin; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\examples\*.*; DestDir: {app}\modules\{#XCOS}\examples; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\demos\*.*; DestDir: {app}\modules\{#XCOS}\demos; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\tests\*.*; DestDir: {app}\modules\{#XCOS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
