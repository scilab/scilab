if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e tdcslib=lib('SCI/macros/tdcs/');save('SCI/macros/tdcs/lib',tdcslib);exit
echo tdcslib=lib('SCI/macros/tdcs/');save('SCI/macros/tdcs/lib',tdcslib);exit>genlib
del tmp_Macros > nul