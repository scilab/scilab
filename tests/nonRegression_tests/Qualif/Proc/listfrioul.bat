@echo off

rem if not "%os%"=="Windows_NT" goto NextStep
REM --- Section spécifique à Windows NT ---
rem color 0F
rem :NextStep
if "%1"=="" goto aide

if exist %envq_log%\listfrioul.log erase %envq_log%\listfrioul.log
 

echo construction de la liste des tests_suites disponibles sur frioul ...
ncftpls -d %envq_log%\listfrioul.log -u qualif -p qualifpwd frioul.inria.fr %envq_work% tests_suite/%1


goto the_end

:aide
echo //

echo //
echo // 

:the_end