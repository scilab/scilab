if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e m2skernellib=lib('SCI/macros/m2sci/kernel/');save('SCI/macros/m2sci/kernel/lib',m2skernellib);exit
echo m2skernellib=lib('SCI/macros/m2sci/kernel');save('SCI/macros/m2sci/kernel/lib',m2skernellib);exit>genlib
del tmp_Macros > nul
