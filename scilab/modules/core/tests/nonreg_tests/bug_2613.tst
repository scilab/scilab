// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2613 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2613
//
// <-- Short Description -->
// Incorrect stack recovery for catched errors in some specific contexts
// (for loops, ..)
// The display comparison is mandatory to check for "stack problem fixed" warning
clear
// reported problem test (loop expression removal)
function y=test()
    y=0
    try
        for j=1:3;
            error(1001);
        end
    catch
        y=1
    end
endfunction

if test()<>1 then pause,end

// temporary arguments removal
//in try/catch
function y=fii,y=3,error(1001),endfunction
function y=foo(a,b,c),y=a+b+c,endfunction
y=0;
try
    foo(1,fii(),2)
catch
    y=1;
end
if y<>1 then pause,end

