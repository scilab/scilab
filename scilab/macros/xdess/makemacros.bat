if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e xdesslib=lib('SCI/macros/xdess/');save('SCI/macros/xdess/lib',xdesslib);exit
echo xdesslib=lib('SCI/macros/xdess/');save('SCI/macros/xdess/lib',xdesslib);exit>genlib
del tmp_Macros