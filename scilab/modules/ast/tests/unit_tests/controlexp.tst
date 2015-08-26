// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- CLI SHELL MODE -->
//

//check break instruction
//in for expression
function res = test_for_break()
    res = 0;
    for i = 1:10
        res = i;
        if res == 5 then
            break;
        end
    end
endfunction

exec(test_for_break);
assert_checktrue(res == 5);

res = test_for_break();
assert_checktrue(res == 5);

//in while expression
function res = test_while_break()
    i = 1;
    res = 0;
    while i <= 10
        res = i;
        if res == 5 then
            break;
        end

        i = i+1;
    end
endfunction

exec(test_while_break);
assert_checktrue(res == 5);

res = test_while_break();
assert_checktrue(res == 5);

//test continue instruction
//in for expression
function res = test_for_continue()
    res = 0;
    for i = 1:10
        if i == 5 then
            continue;
        end
        res = res + i;
    end
endfunction

exec(test_for_continue);
assert_checktrue(res == sum(1:4) + sum(6:10));

res = test_for_continue();
assert_checktrue(res == sum(1:4) + sum(6:10));

//in while expression
function res = test_while_continue()
    i = 1;
    res = 0;
    while i <= 10
        if i == 5 then
            i = i+1;
            continue;
        end
        res = res + i;
        i = i+1;
    end
endfunction

exec(test_while_continue);
assert_checktrue(res == sum(1:4) + sum(6:10));

res = test_while_continue();
assert_checktrue(res == sum(1:4) + sum(6:10));
