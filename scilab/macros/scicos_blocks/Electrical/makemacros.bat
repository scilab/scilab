if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e scselectricallib=lib('SCI/macros/scicos_blocks/Electrical/');save('SCI/macros/scicos_blocks/Electrical/lib',scselectricallib);exit
echo scselectricallib=lib('SCI/macros/scicos_blocks/Electrical');save('SCI/macros/scicos_blocks/Electrical/lib',scselectricallib);exit>genlib
del tmp_Macros > nul
@dir /B *.mo >models
..\..\..\bin\scilex.exe -ns -nb -nwni -f ..\..\..\util\genmoc.sce