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
; scinotes module
;--------------------------------------------------------------------------------------------------------------
;
#define SCINOTES "scinotes"
;
Source: bin\sci{#SCINOTES}.dll; DestDir: {app}\bin; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\jar\org.scilab.modules.{#SCINOTES}.jar; DestDir: {app}\modules\{#SCINOTES}\jar; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\license.txt; DestDir: {app}\modules\{#SCINOTES}; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\sci_gateway\{#SCINOTES}_gateway.xml; DestDir: {app}\modules\{#SCINOTES}\sci_gateway; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\etc\{#SCINOTES}.quit; DestDir: {app}\modules\{#SCINOTES}\etc; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\etc\{#SCINOTES}.start; DestDir: {app}\modules\{#SCINOTES}\etc; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\etc\*.xml; DestDir: {app}\modules\{#SCINOTES}\etc; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\etc\*.xsl; DestDir: {app}\modules\{#SCINOTES}\etc; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\macros\lib; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\macros\*.sci; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\macros\*.bin; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\macros\buildmacros.bat; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SCINOTES}\macros; Components: {#COMPN_SCINOTES}
;
;Source: modules\{#SCINOTES}\includes\*.h; DestDir: {app}\modules\{#SCINOTES}\includes; Components: {#COMPN_SCINOTES}
;
;
;Source: modules\{#SCINOTES}\demos\*.*; DestDir: {app}\modules\{#SCINOTES}\demos; Flags: recursesubdirs; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\tests\*.*; DestDir: {app}\modules\{#SCINOTES}\tests; Flags: recursesubdirs; Components: {#COMPN_SCINOTES} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
