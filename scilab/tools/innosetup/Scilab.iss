;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
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
#define BinariesSourcePath "D:\GIT-scilab-master\scilab\scilab"
#define ScilabBaseFilename "scilab-master"
#define ScilabBaseDirectory "scilab-master"
#define ScilabName "scilab-master"
#define ScilabVersion "5.4.0"
#define CurrentYear "2011"
#define javaSpecificationVersion "1.6"
#define javaUsed "1.6.0_XX"
#define TESTS_SIZE 14364741
#define CHM_SIZE 28135014
;#define WITH_MODELICAC
;#define SCILAB_X64
;#define SCILAB_WITHOUT_JRE
;#define SCILAB_F2C
;-------------------------------------------------------------------------------
#define MODULES_LIST_WEB_PAGE 'http://wiki.scilab.org/howto/install/windows'
#define MKL_DOWNLOAD_HTTP 'http://fileexchange.scilab.org/toolboxes/MKL'
#define MKL_BLASLAPACK_PACKAGENAME 'blas-lapack-mkl-5.4-win32.zip'
#define MKL_COMMONS_PACKAGENAME 'commons-mkl-5.4-win32.zip'
#define MKL_FFTW_PACKAGENAME 'fftw-mkl-5.4-win32.zip'
#define CHM_DOWNLOAD_HTTP 'http://fileexchange.scilab.org/toolboxes/CHM'
#define CHM_PACKAGENAME 'scilab-help-chm-5.4.zip'
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
#include BinariesSourcePath + "\tools\innosetup\code_unzip.iss"
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
OutputExeFilename=D:\Scilab\Output\{#ScilabBaseFilename}.exe
#else
OutputExeFilename=D:\Scilab\Output\{#ScilabBaseFilename}-nojre.exe
#endif
;-------------------------------------------------------------------------------
;
