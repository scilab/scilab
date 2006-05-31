@echo off

REM --- création de la variable d'environnement version de scilab et du repertoire log
set envq_ver=scilab-2.7.2

set envq_qualif=e:\qualif
REM set envq_proc=e:\qualif\proc
REM set envq_work=e:\qualif\work
REM set envq_log=e:\qualif\log

REM --- ajout dans le path du chemin d'installation de la version de scilab a qualifier et du repertoire ou sont installees les procedures necessaires
PATH=%PATH%;%ProgramFiles%\%envq_ver%\bin;%envq_qualif%\proc


REM --- création du repertoire version sous le repertoire log 
if not exist %envq_qualif%\log\%envq_ver% mkdir %envq_qualif%\log\%envq_ver%







