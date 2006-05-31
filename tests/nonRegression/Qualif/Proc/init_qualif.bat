@echo off

REM --- modifier la couleur et le prompt dos
color 71

REM --- positionner paths et variables d'environnement
set envq_qualif=e:\qualif
set envq_proc=e:\qualif\proc
set envq_work=e:\qualif\work
set envq_log=e:\qualif\log

REM --- init version Scilab à tester et path
call init_version_and_path.bat

REM --- prise en compte de la version Scilab dans le prompt
rem PROMPT=$C%envq_ver%$F$S$P$G

REM --- modification du titre de la fenetre DOS
title Env. Qualification Scilab - version %envq_ver%

REM --- création du repertoire version sous le repertoire log 
if not exist %envq_log%\%envq_ver% mkdir %envq_log%\%envq_ver%

REM --- afficher les commandes possibles dans l'environnement de qualification copyftp runtest copyrun
REM call aide.bat





