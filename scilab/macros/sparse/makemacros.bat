if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e sparselib=lib('SCI/macros/sparse/');save('SCI/macros/sparse/lib',sparselib);exit
echo sparselib=lib('SCI/macros/sparse/');save('SCI/macros/sparse/lib',sparselib);exit>genlib
del tmp_Macros > nul