;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
Source: modules\{#SCINOTES}\etc\assistant\*.md; DestDir: {app}\modules\{#SCINOTES}\etc\assistant; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\etc\assistant\agents.json; DestDir: {app}\modules\{#SCINOTES}\etc\assistant; Components: {#COMPN_SCINOTES}
Source: modules\{#SCINOTES}\etc\assistant\skills\*; DestDir: {app}\modules\{#SCINOTES}\etc\assistant\skills; Flags: recursesubdirs; Components: {#COMPN_SCINOTES}
;
; Monaco-based editor surface: our glue page and the Monaco thirdparty bundle.
Source: modules\{#SCINOTES}\src\web\*; DestDir: {app}\modules\{#SCINOTES}\src\web; Flags: recursesubdirs createallsubdirs; Components: {#COMPN_SCINOTES}
Source: thirdparty\web\monaco-editor\*; DestDir: {app}\thirdparty\web\monaco-editor; Flags: recursesubdirs createallsubdirs; Components: {#COMPN_SCINOTES}
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
Source: modules\{#SCINOTES}\gui\assistant\*.*; DestDir: {app}\modules\{#SCINOTES}\gui\assistant; Flags: recursesubdirs; Components: {#COMPN_SCINOTES}
;
Source: modules\{#SCINOTES}\tests\*.*; DestDir: {app}\modules\{#SCINOTES}\tests; Flags: recursesubdirs; Components: {#COMPN_SCINOTES} and {#COMPN_TESTS}
;
Source: modules\{#SCINOTES}\help\*.*; DestDir: {app}\modules\{#SCINOTES}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
