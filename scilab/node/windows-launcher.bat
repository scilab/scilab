@echo off
setlocal
set SCIPATH=%~dp0\..
set SCIFILES=%SCIPATH%/node
node scilab_server.js
