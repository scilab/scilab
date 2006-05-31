@echo off

REM --- affiche a l ecran la derniere ligne du fichier log de la batterie passee en parametre

if "%1"=="" goto saisie

:encore
if "%1"=="" goto the_end
tail --lines=1 --silent %envq_log%\%envq_ver%\%1.log
shift
goto encore

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