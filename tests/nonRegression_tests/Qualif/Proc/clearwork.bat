@echo off

REM --- supprime une, plusieurs ou toutesles batteries de tests du reperoire work local

if "%1"=="--help" goto aide


REM --- LIST : paramètre constitué de la liste de tests-suites demandées
set LIST=

:continue
if "%1"=="" goto lancer_une

for /D %%j in (%envq_qualif%\work\%1) do set LIST=!LIST! %%~nj
shift
goto continue

REM --- lancer chaque tests-suite une par une
:lancer_une
echo liste des tests-suites :%LIST%
echo ------------------------

for %%i in (%LIST%) do echo delete %%i ... & rmdir /s /q %envq_qualif%\work\%%i

goto the_end

:aide
echo //
echo // CLEARWORK
echo // Supprime une, plusieurs ou toutesles batteries de tests sous le repertoire work :
echo // 
echo //  - toutes les tests_suites faire : clearwork *
echo //  - certaines tests_suites faire  : clearwork nonreg* tests_20?? scicos
echo //  - une tests-suite particuliere  : clearwork nonreg_2-7-1
echo //
echo // 

:the_end




