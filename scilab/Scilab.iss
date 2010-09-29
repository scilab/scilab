;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; WARNINGS !!!
; This script MUST be generated
; see SCI/tools/innosetup/Create_Iss.sce
; These current version is ONLY for Dev
;-------------------------------------------------------------------------------
#define BinariesSourcePath "D:\GIT-yasp\scilab\scilab"
#define ScilabBaseFilename "scilab-branch-YaSp"
#define ScilabBaseDirectory "scilab-branch-YaSp"
#define ScilabName "scilab-branch-YaSp"
#define ScilabVersion "6.0.0"
#define CurrentYear "2010"
#define javaSpecificationVersion "1.6"
#define TESTS_SIZE             14381650
#define WITH_MODELICAC
;#define WITH_CHM
#define SCILAB_X64
;#define SCILAB_WITHOUT_JRE
;-------------------------------------------------------------------------------
#define MKL_DOWNLOAD_HTTP "http://www.scilab.org/download/master"
#define MKL_FFTW_DOWNLOAD_HTTP "http://www.scilab.org/download/master"
#define FFTW_WEB_PAGE 'http://www.scilab.org/download/index_download.php'
#define MODULES_LIST_WEB_PAGE 'http://wiki.scilab.org/howto/install/windows'
#define DOTNET_WEB_PAGE 'http://www.microsoft.com/downloads/details.aspx?FamilyID=0856eacb-4362-4b0d-8edd-aab15c5e04f5&DisplayLang=en'
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
#include BinariesSourcePath + "\tools\innosetup\code_download.iss"
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
OutputExeFilename=BinariesSourcePath + "{#ScilabBaseFilename}.exe"
#else
OutputExeFilename=BinariesSourcePath + "{#ScilabBaseFilename}-nojre.exe"
#endif
;-------------------------------------------------------------------------------
;
