if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\bin\scilex.exe -ns -nwni -e soundlib=lib('SCI/macros/sound/');save('SCI/macros/sound/lib',soundlib);exit
echo soundlib=lib('SCI/macros/sound/');save('SCI/macros/sound/lib',soundlib);exit>genlib
del tmp_Macros > nul