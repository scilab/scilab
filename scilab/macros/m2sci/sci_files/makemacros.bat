if EXIST names del names > nul
echo mode(0);>tmp_Macros
for %%f in (*.sci) do echo %%~nf>>names
for %%f in (*.sci) do echo getf('%%f'),save('%%~nf.bin'),clear ;>>tmp_Macros
echo exit;>>tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_Macros
..\..\..\bin\scilex.exe -ns -nwni -e m2ssci_fileslib=lib('SCI/macros/m2sci/sci_files/');save('SCI/macros/m2sci/sci_files/lib',m2ssci_fileslib);exit
echo m2ssci_fileslib=lib('SCI/macros/m2sci/sci_files');save('SCI/macros/m2sci/sci_files/lib',m2ssci_fileslib);exit>genlib
del tmp_Macros > nul
