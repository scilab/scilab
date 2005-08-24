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
echo genlib('scshydraulicslib','SCI/macros/scicos_blocks/hydraulics'); >> tmp_BuildMacros
echo exit; >> tmp_BuildMacros
..\..\..\bin\scilex.exe -ns -nwni -f tmp_BuildMacros

@dir /B *.mo >models
..\..\..\bin\scilex.exe -ns -nb -nwni -f ..\..\..\util\genmoc.sce

if EXIST tmp_BuildMacros del tmp_BuildMacros > nul