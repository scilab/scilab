if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e s2flib=lib('SCI/macros/sci2for/');save('SCI/macros/sci2for/lib',s2flib);exit
echo s2flib=lib('SCI/macros/sci2for/');save('SCI/macros/sci2for/lib',s2flib);exit>genlib
del tmp_Macros > nul