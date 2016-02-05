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
; demo_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define DEMO_TOOLS "demo_tools"
;
Source: modules\{#DEMO_TOOLS}\license.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.quit; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.start; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\images\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\images; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\lib; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\gui\*.xml; DestDir: {app}\modules\{#DEMO_TOOLS}\gui; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DEMO_TOOLS}\examples\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\tests\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
