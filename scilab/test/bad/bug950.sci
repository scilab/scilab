// Non-regression test file for bug 950
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

mode(-1);
clear

try
    function [] = Select (Type)
        select Type, n
        case 1 then
            disp ("1")
        case 2 then
            disp ("2")
        end;
    endfunction;
catch
    ok=%t
end
affich_result(ok,950);
