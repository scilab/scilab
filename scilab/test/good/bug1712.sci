// Non-regression test file for bug 1712
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

mode(-1);
clear

hm=hypermat([2 2 2],1:8);
test1 = execstr("hm>min(hm) & hm<max(hm)","errcatch");
test2 = execstr("hm==min(hm) | hm==max(hm)","errcatch");

if( (test1 == 0) & (test2 == 0) ) then
    affich_result(%T,1712);
else
    affich_result(%F,1712);
end

clear