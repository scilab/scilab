@nmake /F makemex.mak /a all
@copy /Y mexlib.dll ..\..\bin\libmex.dll 
@copy /Y mexlib.dll ..\..\bin\libmx.dll
@copy /Y mexlib.dll ..\..\bin\libmat.dll
@del /Q mexlib.def
@del /Q mexlib.dll
@del /Q mexlib.ilib
@del /Q mexlib.exp