@echo off

rem if not "%os%"=="Windows_NT" goto NextStep
REM --- Section spécifique à Windows NT ---
rem color 0F
rem :NextStep

ncftpget -d %envq_qualif%\log\copyftp.log -u qualif -p qualifpwd frioul.inria.fr %envq_qualif%\work tests_suite/frioul_tests_suite_list.txt
