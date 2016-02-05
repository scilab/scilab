// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

// <-- CLI SHELL MODE -->

function v = while_return()
    v = 0;
    while v == 0
        v = 1;
        return
        v = 2;
    end
endfunction

function v = while_if_return()
    v = 0;
    flag = 0;
    while v == 0
        if v == 0 then
            v = 1;
            return
            v = 2;
        end
    end
    v = 3;
endfunction

function v = for_return()
    v = 0;
    for i = 1:1
        v = 1;
        return
        v = 2;
    end
endfunction

function v = for_if_return()
    v = 0;
    flag = 0;
    for i = 1:1
        if v == 0 then
            v = 1;
            return
            v = 2;
        end
    end
    v = 3;
endfunction

function y=foo()
    y = [];
    return y;
end

assert_checkequal(while_return(), 1);
assert_checkequal(while_if_return(), 1);
assert_checkequal(for_return(), 1);
assert_checkequal(for_if_return(), 1);
assert_checkerror("foo()", _("With input arguments, return / resume expects output arguments."));
