@echo off
if "%1"=="" goto aide

set MYLIST=

:continue
if "%1"=="" goto suite
set MYLIST=%MYLIST% %1
shift
goto continue

:suite 
call copyftp %MYLIST%
call runtest %MYLIST%

goto the_end

:aide
echo //
echo // COPYRUN
echo // Copie puis execute une ou plusieurs batteries de tests dans l'environnement de qualification Scilab (les meta-caracteres sont acceptes) :
echo //
echo // copyrun tests_suite [tests_suite]
echo // copyrun *
echo // copyrun n* sc?cos autre 
echo //
echo //

:the_end