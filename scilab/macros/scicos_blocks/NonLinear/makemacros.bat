if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e scsnonlinearlib=lib('SCI/macros/scicos_blocks/NonLinear/');save('SCI/macros/scicos_blocks/NonLinear/lib',scsnonlinearlib);exit
echo scsnonlinearlib=lib('SCI/macros/scicos_blocks/NonLinear');save('SCI/macros/scicos_blocks/NonLinear/lib',scsnonlinearlib);exit>genlib
del tmp_Macros > nul
