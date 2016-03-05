;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
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
#define BinariesSourcePath "D:\GIT-scilab-master\scilab\scilab"
#define ScilabBaseFilename "scilab-master"
#define ScilabBaseDirectory "scilab-master"
#define ScilabName "scilab-master"
#define ScilabVersion "5.4.0"
#define CurrentYear "2011"
#define javaSpecificationVersion "1.6"
#define javaUsed "1.6.0_XX"
#define TESTS_SIZE 14364741
;#define WITH_MODELICAC
;#define SCILAB_X64
;#define SCILAB_WITHOUT_JRE
;#define SCILAB_F2C
;-------------------------------------------------------------------------------
#define MODULES_LIST_WEB_PAGE 'http://wiki.scilab.org/howto/install/windows'
;-------------------------------------------------------------------------------
[CustomMessages]
#include BinariesSourcePath + "\tools\innosetup\custommessages.iss"
;-------------------------------------------------------------------------------
[Setup]
#include BinariesSourcePath + "\tools\innosetup\setup.iss"
;-------------------------------------------------------------------------------
[Languages]
#include BinariesSourcePath + "\tools\innosetup\languages.iss"
;-------------------------------------------------------------------------------
[Types]
#include BinariesSourcePath + "\tools\innosetup\types.iss"
;-------------------------------------------------------------------------------
[Components]
#include BinariesSourcePath + "\tools\innosetup\components.iss"
;
;-------------------------------------------------------------------------------
[Files]
#include BinariesSourcePath + "\tools\innosetup\files.iss"
;
;-------------------------------------------------------------------------------
[Icons]
#include BinariesSourcePath + "\tools\innosetup\icons.iss"
;-------------------------------------------------------------------------------
[INI]
#include BinariesSourcePath + "\tools\innosetup\ini.iss"
;-------------------------------------------------------------------------------
[Tasks]
#include BinariesSourcePath + "\tools\innosetup\tasks.iss"
;-------------------------------------------------------------------------------
[Registry]
#include BinariesSourcePath + "\tools\innosetup\registry.iss"
;-------------------------------------------------------------------------------
[Code]
#include BinariesSourcePath + "\tools\innosetup\code_modules.iss"
#include BinariesSourcePath + "\tools\innosetup\code.iss"
[Run]
;-------------------------------------------------------------------------------
#include BinariesSourcePath + "\tools\innosetup\run.iss"
;-------------------------------------------------------------------------------
[UninstallDelete]
#include BinariesSourcePath + "\tools\innosetup\uninstalldelete.iss"
;-------------------------------------------------------------------------------
[_ISTool]
#ifndef SCILAB_WITHOUT_JRE
OutputExeFilename=D:\Scilab\Output\{#ScilabBaseFilename}.exe
#else
OutputExeFilename=D:\Scilab\Output\{#ScilabBaseFilename}-nojre.exe
#endif
;-------------------------------------------------------------------------------
;
