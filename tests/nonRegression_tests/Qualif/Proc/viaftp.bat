@echo off
REM --- recuperere le repertoire courant

if "%1"=="" goto aide

if exist %envq_proc%\ncftp.cfg erase %envq_proc%\ncftp.cfg

REM --- liste des tests-suites à copier
echo lcd work > %envq_proc%\ncftp.cfg
echo cd tests_suite >> %envq_proc%\ncftp.cfg

:encore
if "%1"=="" goto suite
echo get -R %1 >> %envq_proc%\ncftp.cfg
shift
goto encore

:suite
echo quit >> %envq_proc%\ncftp.cfg

attrib -H login.cfg
copy login.cfg+ncftp.cfg bip.cfg
attrib +H login.cfg

rem ncftp < ncftp.cfg
goto the_end

:aide
echo afficher l'aide

REM --- redoner le repertoire initial

:the_end
