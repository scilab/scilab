// Non-regression test file for bug 1239
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 21 Mar 2005

mode(-1);
clear;

if( (nanmax([%nan, -1]) <> -1) | (nanmax([%nan, 1]) <> 1) | (nanmin([%nan, -1]) <> -1) | (nanmin([%nan, 1]) <> 1) ) then
	affich_result(%F,1239);
else
	affich_result(%T,1239);
end

clear
