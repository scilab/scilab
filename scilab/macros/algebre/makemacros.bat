if EXIST names del names >nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e alglib=lib('SCI/macros/algebre/');save('SCI/macros/algebre/lib',alglib);exit
echo alglib=lib('SCI/macros/algebre/');save('SCI/macros/algebre/lib',alglib);exit>genlib
del tmp_Macros > nul