;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
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

; // These files MUST be generated in final version of Scilab 6
Source: etc\modules.xml; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
;
Source: etc\logging.properties; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
; root directory
Source: CHANGES.md; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: COPYING; DestDir: {app}; Components: {#COMPN_SCILAB}
Source: COPYING-BSD; DestDir: {app}; Components: {#COMPN_SCILAB}
; (don't include README, as it's only about Unix)
;
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
Source: locale\ca_ES\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ca_ES\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\cs_CZ\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\cs_CZ\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\de_DE\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\de_DE\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\en_US\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\en_US\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\es_ES\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\es_ES\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\fr_FR\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\fr_FR\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\it_IT\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\it_IT\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\ja_JP\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ja_JP\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\pl_PL\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\pl_PL\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\pt_BR\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\pt_BR\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\ru_RU\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\ru_RU\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\uk_UA\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\uk_UA\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\zh_CN\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\zh_CN\LC_MESSAGES; Components: {#COMPN_SCILAB}
Source: locale\zh_TW\LC_MESSAGES\scilab.mo ; DestDir: {app}\locale\zh_TW\LC_MESSAGES; Components: {#COMPN_SCILAB}
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
