if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e m2spercentlib=lib('SCI/macros/m2sci/percent/');save('SCI/macros/m2sci/percent/lib',m2spercentlib);exit
echo m2spercentlib=lib('SCI/macros/m2sci/percent');save('SCI/macros/m2sci/percent/lib',m2spercentlib);exit>genlib
del tmp_Macros > nul
