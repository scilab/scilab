;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------
; default files embedded with scilab
;-------------------------------------------------------------------------------
;
Source: tools\zip\unzip.exe; Flags: dontcopy
Source: tools\innosetup\isxdl.dll; DestDir: {tmp}; Flags: dontcopy

;
; etc directory
Source: etc\scilab.start; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
Source: etc\scilab.quit; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
Source: etc\logging.properties; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
; root directory
Source: RELEASE_NOTES*.*; DestDir: {app}; Components: {#COMPN_SCILAB}
;
Source: CHANGES*.*; DestDir: {app}; Components: {#COMPN_SCILAB}
;Source: licence.txt; DestDir: {app}; Components: {#COMPN_SCILAB}
;Source: license.txt; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: COPYING; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: COPYING-FR; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: COPYING-BSD; DestDir: {app}; Components: {#COMPN_SCILAB}
;
Source: Readme_Visual.txt; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: README_Windows.txt; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: ACKNOWLEDGEMENTS; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: Version.incl; DestDir: {app}; Components: {#COMPN_SCILAB}
;
; bin directory
#include BinariesSourcePath + "\bin\bin.iss"
;
; contrib directory
Source: contrib\loader.sce; DestDir: {app}\contrib; Components: {#COMPN_SCILAB}
;
; locale directory
Source: locale\en_US\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\en_US\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\fr_FR\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\fr_FR\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\zh_TW\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\zh_TW\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\zh_CN\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\zh_CN\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\ru_RU\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ru_RU\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\de_DE\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\de_DE\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\ca_ES\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ca_ES\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\es_ES\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\es_ES\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\pt_BR\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\pt_BR\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\ja_JP\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ja_JP\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\it_IT\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\it_IT\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\uk_UA\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\uk_UA\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\pl_PL\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\pl_PL\LC_MESSAGES; Components: {#COMPN_SCILAB}
;
; libs directory
#include BinariesSourcePath + "\libs\libs.iss"
; java directory
#ifndef SCILAB_WITHOUT_JRE
Source: java\JRE\*.*; DestDir: {app}\java\JRE; Flags: recursesubdirs; Components: {#COMPN_JRE}
#endif
;
;-------------------------------------------------------------------------------
;
