if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e metalib=lib('SCI/macros/metanet/');save('SCI/macros/metanet/lib',metalib);exit
echo metalib=lib('SCI/macros/metanet/');save('SCI/macros/metanet/lib',metalib);exit>genlib
del tmp_Macros > nul