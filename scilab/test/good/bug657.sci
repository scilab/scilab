// Non-regression test file for bug 657
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

mode(-1);
clear

test1 = max(int8(10),2);
err = execstr("isdir(test1)","errcatch");
test2 = max(int8(10),2);
pwd
test3 = max(int8(10),2);

if( (test1 == 10) & (err == 246) & (test2 == 10) & (test3 == 10) ) then
    affich_result(%T,657);
else
    affich_result(%F,657);
end

clear