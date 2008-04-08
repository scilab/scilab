;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
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
; metanet module
;--------------------------------------------------------------------------------------------------------------
;
#define METANET "metanet"
;
Source: bin\metanet_f.dll; DestDir: {app}\bin; Components: {#COMPN_METANET}
Source: bin\metanet.dll; DestDir: {app}\bin; DestName: metanet.dll; Components: {#COMPN_METANET}
Source: bin\nometanet.dll; DestDir: {app}\bin; DestName: metanet.dll; Components: not ({#COMPN_METANET})
;
Source: modules\{#METANET}\VERSION.xml; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\readme.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\license.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\changelog.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\sci_gateway\metanet_gateway.xml; DestDir: {app}\modules\{#METANET}\sci_gateway; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\etc\{#METANET}.quit; DestDir: {app}\modules\{#METANET}\etc; Components: {#COMPN_METANET}
Source: modules\{#METANET}\etc\{#METANET}.start; DestDir: {app}\modules\{#METANET}\etc; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\help\addchapter.sce; DestDir: {app}\modules\{#METANET}\help; Flags: recursesubdirs; Components:  {#COMPN_METANET}
;
Source: modules\{#METANET}\includes\*.h; DestDir: {app}\modules\{#METANET}\includes; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\macros\editor\lib; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\names; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\*.bin; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\*.sci; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\buildmacros.sce; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\buildmacros.bat; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\editor\cleanmacros.bat; DestDir: {app}\modules\{#METANET}\macros\editor; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\macros\graph_tools\lib; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\names; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\*.bin; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\*.sci; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\buildmacros.sce; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\buildmacros.bat; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\graph_tools\cleanmacros.bat; DestDir: {app}\modules\{#METANET}\macros\graph_tools; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\macros\buildmacros.sce; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\buildmacros.bat; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\demos\*.*; DestDir: {app}\modules\{#METANET}\demos; Flags: recursesubdirs; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\tests\*.*; DestDir: {app}\modules\{#METANET}\tests; Flags: recursesubdirs; Components: {#COMPN_METANET} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
