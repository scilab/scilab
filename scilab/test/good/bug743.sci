// Non-regression test file for bug 743
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 21 octobre 2005

mode(-1);
clear;

x=int32(253);
if ((x&int32(1))<>int32(0)) then
	affich_result(%T,753);
else
	affich_result(%F,753);
end

clear
