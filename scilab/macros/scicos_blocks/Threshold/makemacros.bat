if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e scsthresholdlib=lib('SCI/macros/scicos_blocks/Threshold/');save('SCI/macros/scicos_blocks/Threshold/lib',scsthresholdlib);exit
echo scsthresholdlib=lib('SCI/macros/scicos_blocks/Threshold');save('SCI/macros/scicos_blocks/Threshold/lib',scsthresholdlib);exit>genlib
del tmp_Macros > nul
