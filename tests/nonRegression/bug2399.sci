// Non-regression test file for bug 2399
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

mode(-1);
clear;

toks = ['noddy';'holden'];
vals = msscanf(-1,toks,'%f');

if( vals == [] ) then
	affich_result(%T,2399);
else
	affich_result(%F,2399);
end

clear
