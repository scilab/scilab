:begin
if "%1" == "" goto all
if "%1" == "all" goto all
if "%1" == "clean" goto clean
:all
@nmake /f makefile.mak all
nmake /f makefile.mak tests
goto Ende
:clean
nmake /f makefile.mak clean
goto Ende
:Ende
