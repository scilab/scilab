@echo off

rem if not "%os%"=="Windows_NT" goto NextStep
REM --- Section spécifique à Windows NT ---
rem color 0F
rem :NextStep
if "%1"=="--help" goto aide

if exist %envq_qualif%\log\copyftp.log erase %envq_qualif%\log\copyftp.log
 
:encore
if "%1"=="" goto the_end
echo copy de %1 ...
ncftpget -R -d %envq_qualif%\log\copyftp.log -u qualif -p qualifpwd frioul.inria.fr %envq_qualif%\work tests_suite/%1
shift
goto encore

rem for /F %%i in ('dir /A:A /O:S /B') do call :action1 %%i

goto the_end

:aide
echo //
echo // COPYFTP
echo // Copie a partir du serveur via ftp (ncftpget) dans le repertoire Work local de l'environnement de qualification Scilab (les meta-caracteres sont acceptes) :
echo // 
echo //  - toutes les tests_suites faire : copyftp *
echo //  - certaines tests_suites faire  : copyftp nonreg* tests_20?? scicos
echo //  - une tests-suite particuliere  : copyftp nonreg_2-7-1
echo //
echo // 

:the_end