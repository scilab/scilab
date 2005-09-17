@echo off
if %1==clean del /S *.bin del /S lib del /S *.moc
if %1==distclean del /S *.bin
if %1==all ..\bin\scilex -nwni -ns -f buildmacros.sce