@echo off

goto dessous

REM --- supprime une, plusieurs ou toutesles batteries de tests du reperoire work local

if "%1"=="" goto aide

:encore
if "%1"=="" goto the_end
rmdir /s /q %envq_work%\%1
if %errorlevel% leq 1 echo delete %1 ...
shift
goto encore


goto the_end

:aide
echo //
echo // CLEARWORK
echo // Supprime toutes les batteries de tests sous le reperoire work :
echo // 
echo //  - toutes les tests_suites faire : clearwork *
echo //  - certaines tests_suites faire  : clearwork nonreg* tests_20?? scicos
echo //  - une tests-suite particuliere  : clearwork nonreg_2-7-1
echo //
echo // 

:the_end


:dessous
rmdir /s /q %envq_work%\*.*



