//==============================================================================
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA 2008
// Date : 8 Feb 2008
//==============================================================================

if execstr("getversion();","errcatch")                                 <> 0 then pause, end
if execstr("[version,opts]=getversion()","errcatch")                   <> 0 then pause, end
if execstr("ver=getversion(''scilab'')","errcatch")                    <> 0 then pause, end
if execstr("verstr=getversion(''scilab'',''string_info'')","errcatch") <> 0 then pause, end

module_list = getmodules();

for i=1:size(module_list,"*")
	if execstr("ver=getversion(''"+module_list(i)+"'')","errcatch")                    <> 0 then pause, end
	if execstr("verstr=getversion(''"+module_list(i)+"'',''string_info'')","errcatch") <> 0 then pause, end
end
