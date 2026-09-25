@echo off
REM Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
REM Copyright (C) 2022 - Dassault Systèmes S.E. - Clément DAVID
REM Copyright (C) 2022 - Dassault Systèmes S.E. - Cédric DELAMARRE
REM Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
REM
REM Builder script for building Scilab on Windows
REM
REM NOTE: log all commands to log files to avoid hitting Gitlab log limit

REM set Visual Studio environment
echo VS2026INSTALLDIR=%VS2026INSTALLDIR%
call "%VS2026INSTALLDIR%\Common7\Tools\VsDevCmd.bat"

echo Scilab for Windows %ARCH% in branch %BRANCH%

REM Create log folder
set LOG_PATH=%SCI_VERSION_STRING%
if not exist %LOG_PATH% mkdir %LOG_PATH%

REM checkout pre-requirements
DEL /q prereq.zip
set OVERRIDE_THIRDPARTY=0

REM custom build for this commit or tag
move /Y prerequirements-%SCI_VERSION_STRING%-windows_%ARCH%.zip prereq.zip
IF NOT EXIST prereq.zip (
    REM custom build for this branch
    copy -a "prerequirements-scilab-branch-${BRANCH}.bin.${ARCH}.tar.xz" "prereq.tar.xz"
)
IF NOT EXIST prereq.zip (
    REM download prebuild for the MR branch
    curl.exe -Lk -o prereq.zip https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-%CI_MERGE_REQUEST_SOURCE_BRANCH_NAME%-windows_%ARCH%.zip
    unzip.exe -qt prereq.zip || DEL /q prereq.zip
)
IF NOT EXIST prereq.zip (
    REM download prebuild for the target branch
    curl.exe -Lk -o prereq.zip https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-%BRANCH%-windows_%ARCH%.zip
    unzip.exe -qt prereq.zip || DEL /q prereq.zip
    set OVERRIDE_THIRDPARTY=1
)
IF NOT EXIST prereq.zip (
    REM fallback to the default branch
    curl.exe -Lk -o prereq.zip https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-%CI_DEFAULT_BRANCH%-windows_%ARCH%.zip
    unzip.exe -qt prereq.zip || DEL /q prereq.zip
    set OVERRIDE_THIRDPARTY=1
)
IF NOT EXIST prereq.zip (
    REM fallback to the main branch
    curl.exe -Lk -o prereq.zip https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-main-windows_%ARCH%.zip
    unzip.exe -qt prereq.zip || DEL /q prereq.zip
    set OVERRIDE_THIRDPARTY=1
)
git clean -fxd scilab/bin scilab/java scilab/libs scilab/modules/tclsci/tcl scilab/thirdparty scilab/tools > nul
del /f /q /a scilab\svn-info.txt scilab\version.txt
unzip -o prereq.zip -d scilab > %LOG_PATH%\build_prereq_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 exit 1

REM display svn revision
type scilab\svn-info.txt
IF %ERRORLEVEL% NEQ 0 exit 1

REM patch thirdparty JARs on WIP Merge-Request
if %OVERRIDE_THIRDPARTY% NEQ 0 (
    curl.exe -Lk -o thirdparty.zip https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements-sources/thirdparty-scilab-branch-%CI_MERGE_REQUEST_SOURCE_BRANCH_NAME%.zip
    unzip.exe -qt thirdparty.zip || DEL /q thirdparty.zip
)
IF EXIST thirdparty.zip (
    rd /s /q scilab\thirdparty\
    mkdir scilab\thirdparty\
    unzip.exe -o thirdparty.zip -d scilab\thirdparty\
)

REM Define environment variables if not defined
IF "%SCILAB_JDK64%"=="" set SCILAB_JDK64=%JAVA_HOME%

cd scilab

REM patch version numbers
sed -i ^
 -e "s/SCI_VERSION_STRING .*/SCI_VERSION_STRING \"%SCI_VERSION_STRING%\"/" ^
 -e "s/SCI_VERSION_WIDE_STRING .*/SCI_VERSION_WIDE_STRING L\"%SCI_VERSION_STRING%\"/" ^
 -e "s/SCI_VERSION_REVISION .*/SCI_VERSION_REVISION \"%CI_COMMIT_SHA%\"/" ^
 -e "s/SCI_VERSION_TIMESTAMP .*/SCI_VERSION_TIMESTAMP %SCI_VERSION_TIMESTAMP%/" ^
 modules\core\includes\version.h.vc
if exist modules\core\includes\version.h sed -i ^
 -e "s/SCI_VERSION_STRING .*/SCI_VERSION_STRING \"%SCI_VERSION_STRING%\"/" ^
 -e "s/SCI_VERSION_WIDE_STRING .*/SCI_VERSION_WIDE_STRING L\"%SCI_VERSION_STRING%\"/" ^
 -e "s/SCI_VERSION_REVISION .*/SCI_VERSION_REVISION \"%CI_COMMIT_SHA%\"/" ^
 -e "s/SCI_VERSION_TIMESTAMP .*/SCI_VERSION_TIMESTAMP %SCI_VERSION_TIMESTAMP%/" ^
 modules\core\includes\version.h
echo SCIVERSION=%SCI_VERSION_STRING% >Version.incl

REM build with Visual Studio and Intel compilers
set SCILAB_SOLUTION=Scilab.sln
set PLATFORM=x64
if "%ARCH%" == "arm64" (
    set SCILAB_SOLUTION=Scilab_f2c.sln
    set PLATFORM=ARM64
)
devenv %SCILAB_SOLUTION% /build "Release|%PLATFORM%" /project dumpexts > ..\%LOG_PATH%\build_dumpexts_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_dumpexts_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1
devenv %SCILAB_SOLUTION% /build "Release|%PLATFORM%" > ..\%LOG_PATH%\build_sln_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_sln_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1
devenv %SCILAB_SOLUTION% /build "Release|%PLATFORM%" /project buildhelp > ..\%LOG_PATH%\build_help_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_help_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1
REM check the pre-generated help images and pack help_images.zip
devenv %SCILAB_SOLUTION% /build "Release|%PLATFORM%" /project buildDoc > ..\%LOG_PATH%\build_doc_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=40 ..\%LOG_PATH%\build_doc_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1
devenv %SCILAB_SOLUTION% /build "Release|%PLATFORM%" /project buildjavadoc > ..\%LOG_PATH%\build_javadoc_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_javadoc_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1

REM Package with Inno Setup 6
if exist "Scilab.iss" del /f "Scilab.iss"
bin\WScilex-cli.exe -nb -f "tools\innosetup\Create_ISS.sce" > ..\%LOG_PATH%\build_iss_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_iss_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1
if not exist "Scilab.iss" exit 1
set ISS_MR=0
if "%CI_PIPELINE_SOURCE%" == "merge_request_event" set ISS_MR=1
"C:\Program Files (x86)\Inno Setup 7\iscc.exe" Scilab.iss /DMR=%ISS_MR% >> ..\%LOG_PATH%\build_iss_%CI_COMMIT_SHORT_SHA%.log
IF %ERRORLEVEL% NEQ 0 tail --lines=20 ..\%LOG_PATH%\build_iss_%CI_COMMIT_SHORT_SHA%.log 1>&2 & exit 1

REM store log on the shared directory
dir /s /b "..\%SCI_VERSION_STRING%\" "%SCILAB_COMMON_PATH%\%SCI_VERSION_STRING%\log\"
copy "..\%SCI_VERSION_STRING%\" "%SCILAB_COMMON_PATH%\%SCI_VERSION_STRING%\log\"
IF %ERRORLEVEL% NEQ 0 exit 1

REM artifact and persistant files
move ".\Output\%SCI_VERSION_STRING%_%ARCH%.exe" "..\%SCI_VERSION_STRING%.bin.%ARCH%.exe"
copy "..\%SCI_VERSION_STRING%.bin.%ARCH%.exe" "%SCILAB_COMMON_PATH%\%SCI_VERSION_STRING%\%SCI_VERSION_STRING%.bin.%ARCH%.exe"
IF %ERRORLEVEL% NEQ 0 exit 1

REM Check for debug DLLs in the Windows build
@echo off
for /r ".\bin\" %%F in (*.dll) do (
    dumpbin /dependents "%%F" /out:dll_dependents_list.txt >nul
    findstr /I /R /C:" libifcoremdd\.dll" /C:" libifcorertd\.dll" /C:" vcruntime[0-9_]*d\.dll" /C:" msvcrtd\.dll" dll_dependents_list.txt
    IF NOT ERRORLEVEL 1 (
        echo WARNING: Found debug DLL dependency in "%%F"
        type dll_dependents_list.txt
        exit 1
    )
)
del dll_dependents_list.txt

exit 0
