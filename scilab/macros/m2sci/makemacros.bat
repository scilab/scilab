if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e m2scilib=lib('SCI/macros/m2sci/');save('SCI/macros/m2sci/lib',m2scilib);exit
echo m2scilib=lib('SCI/macros/m2sci/');save('SCI/macros/m2sci/lib',m2scilib);exit>genlib
del tmp_Macros > nul