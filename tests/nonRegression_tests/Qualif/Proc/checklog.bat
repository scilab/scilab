@echo off

REM --- Echo Analyse des log tests_suites resultats

find "{ FIN TEST SUITE " %envq_qualif%\log\%envq_ver%\%1.log
if %errorlevel% == 0 (

REM ---la chaine FIN TEST est trouvee , la tests suite est achevee , chercher si la chaine FAILED existe
call :suite %1

) else (

   if %errorlevel% == 1 (
REM ---chaine FIN TEST pas trouvee , la test suite pas terminee
   echo %1 TEST SUITE --------------- NOT COMPLETED >> %envq_qualif%\log\%envq_ver%\%1.log
   )

)

goto fin

:suite

   find /i "FAILED" %envq_qualif%\log\%envq_ver%\%1.log
   if %errorlevel% == 0 (
REM ---la chaine FAILED est trouvee , la test suite est incorrecte
   echo %1 TEST SUITE --------------- FAILED >> %envq_qualif%\log\%envq_ver%\%1.log

   ) else (
REM la chaine FAILED n est pas trouvee , la test suite est correcte
echo %1 TEST SUITE *** PASSED *** >> %envq_qualif%\log\%envq_ver%\%1.log
   )


:fin