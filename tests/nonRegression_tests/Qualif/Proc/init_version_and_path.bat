@echo off

REM --- création de la variable d'environnement version de scilab
set envq_ver=scilab-2.7.2

REM --- ajout dans le path du chemin d'installation de la version de scilab a qualifier et du repertoire ou sont installees les procedures necessaires
PATH=%PATH%;%ProgramFiles%\%envq_ver%\bin;%envq_proc%