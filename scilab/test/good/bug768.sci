// Non-regression test file for bug 768
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

mode(-1);
clear

err_number = execstr("window(''ch'',128,[0.25 0.1])","errcatch");
err_message = lasterror();

if( err_message == "Parameter par should be [dp,df] where one of dp, df is equal to -1             ") then
    affich_result(%T,768);
else
    affich_result(%F,768);
end

clear
