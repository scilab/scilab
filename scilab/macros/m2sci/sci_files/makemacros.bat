@echo off
if EXIST *.bin del *.bin > nul
if EXIST lib del lib > nul
if EXIST names del names > nul

echo SCI=getenv('SCI'); > tmp_BuildMacros
echo mode(0); >> tmp_BuildMacros
echo load('SCI/macros/util/lib'); >> tmp_BuildMacros
echo load('SCI/macros/percent/lib'); >> tmp_BuildMacros
echo MSDOS = (getos() == "Windows"); >> tmp_BuildMacros
echo TMPDIR=getenv('TMPDIR'); >> tmp_BuildMacros
echo genlib('m2ssci_fileslib','SCI/macros/m2sci/sci_files'); >> tmp_BuildMacros
echo exit; >> tmp_BuildMacros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_BuildMacros

if EXIST tmp_BuildMacros del tmp_BuildMacros > nul