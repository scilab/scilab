@echo off

cls
echo //
echo //  commandes disponibles : COPYFTP - RUNTEST - COPYRUN - CLEARWORK - CLEARLOG
echo //
echo //  pour une aide faire : AIDE [all] ou lancer une commande sans parametre
echo //
echo //        

if "%1"=="all" goto suite
if "%1"=="ALL" goto suite
goto the_end

:suite
cls
call copyftp
call runtest
call copyrun
call clearwork
call clearlog

:the_end