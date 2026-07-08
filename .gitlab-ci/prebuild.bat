@echo off
REM Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
REM Copyright (C) 2022 - Dassault Systèmes S.E. - Clément DAVID
REM Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
REM
REM Builder script for building Scilab prerequirements on Windows

if "%1" == "" (
    echo This script compiles dependencies of Scilab for Windows x64.
    echo.
    echo Syntax: %~n0 ^<dependency^> with dependency equal to:
    echo  - 'version': display versions of dependencies,
    echo  - 'download': download all dependencies,
    echo  - 'copy': download all dependencies,
    echo  - 'fromscratch': 'download' + 'copy'.
    echo.
    exit(42)
)

echo Scilab prerequirements for Windows %ARCH% in branch %BRANCH%

set LOGDIR="%SCI_VERSION_STRING%"
if exist "%LOGDIR%" (
    echo logging into existing "%LOGDIR%"
) else (
    mkdir "%LOGDIR%" && echo "%LOGDIR%" created
)

rem ################################
rem ##### DEPENDENCIES VERSION #####
rem ################################
set SVN_REVISION=30164

rem ###############################
rem ##### ARGUMENT MANAGEMENT #####
rem ###############################
:loop
    if "%1" == "" goto :done
    if "%1" == "version" (
        call :make_versions
    )
    if "%1" == "fromscratch" (
        call :download_prereqs
        call :make_versions
        call :copy
    )
    if "%1" == "download" (
        call :download_prereqs
    )
    if "%1" == "copy" (
        call :copy
    )
    shift
goto :loop

rem #####################
rem ##### FUNCTIONS #####
rem #####################

:make_versions
    echo SVN_REVISION = %SVN_REVISION%
    echo SVN_REVISION = %SVN_REVISION% > "%LOGDIR%/prebuild_svn_revision.log"
    if exist prereq.zip (
        unzip -l prereq.zip > "%LOGDIR%/prebuild_svn_revision.log"
    )
    goto :eof

:download_prereqs
    set SVN_REVISION_PREREQS=https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-svn-revision-%SVN_REVISION%-windows_%ARCH%.zip
    echo Downloading %SVN_REVISION_PREREQS%...
    curl.exe -f -L -k -o prereq.zip %SVN_REVISION_PREREQS%
    IF %ERRORLEVEL% NEQ 0 (
        echo %SVN_REVISION_PREREQS% not found.
        goto :download_default_prereqs
    )
    unzip.exe -qt prereq.zip
    goto :eof

:download_default_prereqs
    rem Fallback on standard prereqs file for current branch
    rem Useful when prebuild.bat is modified for another reason than SVN_REVISION change and SVN_REVISION prereqs are not more available on Outscale
    set STD_PREREQS=https://oos.eu-west-2.outscale.com/scilab-releases-dev/prerequirements/prerequirements-scilab-branch-%BRANCH%-windows_%ARCH%.zip
    echo Downloading %STD_PREREQS%...
    curl.exe -L -k -o prereq.zip %STD_PREREQS%
    unzip.exe -qt prereq.zip
    goto :eof

:copy
    copy /Y prereq.zip prerequirements-%SCI_VERSION_STRING%-windows_%ARCH%.zip
    goto :eof

:done
