// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14047 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14047
//
// <-- Short Description -->
// wrong behaviour of break ( continue ) in if outside of loop

function y = testBreak()
    if 1 then
        y = 2;
        break;
        y = 3;
    end
endfunction

assert_checkequal(testBreak(), 3);


function y = testContinue()
    if 1 then
        y = 2;
        continue;
        y = 3;
    end
endfunction

assert_checkequal(testContinue(), 3);

clear testBreak testContinue
//check normal behaviour
function y = testBreak()
    y = 0;
    for i = 1:5
        if i == 3 then
            break;
        end

        y = y + 1;
    end
endfunction

assert_checkequal(testBreak(), 2);

function y = testContinue()
    y = 0;
    for i = 1:5
        if i == 3 then
            continue;
        end

        y = y + 1;
    end
endfunction

assert_checkequal(testContinue(), 4);
