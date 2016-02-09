@echo off
setlocal
set SCI=%~dp0\..
set PATH=%SCI%\bin;%PATH%
node scilab_server.js
