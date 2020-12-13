// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16565 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16565
//
// <-- Short Description -->
// edit(user_defined_function) does not yield reliable code

function a = test(c,d)
    // http://bugzilla.scilab.org/16565 :
    y = c{1}
    // y = c{2}    // yields = c{1}
    // y = c{1,2}  // yields = c{1,1}
    y = c{2,1}
    y{3} = 4
    y{2,1} = 3
    y{1,2} = 3
    plot
    // http://bugzilla.scilab.org/16576 :
    b = gcf().children.axes_reverse(:,2)
    //
    while a > 0
        a = a - 0.1
    end
endfunction

try // We just need edit to create the file(). Scinotes can't be opened
    edit test
end

File = pathconvert(TMPDIR) + "test.sci";
t = mgetl(File);
Ref = [
    "function a = test(c, d)"
    "    // http://bugzilla.scilab.org/16565 :"
    "    y = c{1}"
    "    // y = c{2}    // yields = c{1}"
    "    // y = c{1,2}  // yields = c{1,1}"
    "    y = c{2, 1}"
    "    y{3} = 4"
    "    y{2, 1} = 3"
    "    y{1, 2} = 3"
    "    plot"
    "    // http://bugzilla.scilab.org/16576 :"
    "    b = gcf().children.axes_reverse(:, 2)"
    "    //"
    "    while (a > 0) do"
    "        a = a - 0.1"
    "    end"
    "endfunction"
    ];
assert_checkequal(t, Ref);
