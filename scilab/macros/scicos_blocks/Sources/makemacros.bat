if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e scssourceslib=lib('SCI/macros/scicos_blocks/Sources/');save('SCI/macros/scicos_blocks/Sources/lib',scssourceslib);exit
echo scssourceslib=lib('SCI/macros/scicos_blocks/Sources');save('SCI/macros/scicos_blocks/Sources/lib',scssourceslib);exit>genlib
del tmp_Macros > nul
