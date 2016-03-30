@echo off
setlocal
set SCIPATH=%~dp0\..
set SCIFILES=%SCIPATH%/node
node-debug -p 1338 scilab_server.js
