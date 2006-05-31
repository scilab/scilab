@echo off

REM --- supprime tous les fichiers log des batteries de tests sous le répertoire log\version scilab

if "%1"=="--help" goto aide
if "%1"=="" goto saisie
 
:encore
if "%1"=="" goto the_end
erase /s /q %envq_qualif%\log\%envq_ver%\%1.log
REM if %errorlevel% leq 1 echo delete %1.log ...
shift
goto encore

goto the_end

:saisie

goto the_end

:aide
echo -            
echo // CLEARLOG
echo // Supprime un, plusieurs ou tous les fichiers log des batteries de tests du repertoire log\version scilab :
echo // 
echo //  - toutes les tests_suites faire : clearlog *
echo //  - certaines tests_suites faire  : clearlog nonreg* tests_20?? scicos
echo //  - une tests-suite particuliere  : clearlog nonreg_2-7-1
echo //
echo // 

:the_end









