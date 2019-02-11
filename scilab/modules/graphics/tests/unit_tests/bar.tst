// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

// Unitary tests of bar()
// ----------------------

// Allowed syntaxes
// ----------------
Y_34 = grand(3,4,'uin',0,6)/2;
for a = ["" "ax, "]     // Targeted axes
    for xy = ["[2 1 1.5]"  "[2 1 1.5]''"  "[], [2 1 1.5]"  "7.5, [2 1 1.5]" ..
              "-1.2:0.8, [2 1 1.5]"  "2.2:1.1:4.4, [2 1 1.5]''" ..
              "(2.2:1.1:4.4)'', [2 1 1.5]''" ..
              "Y_34"  "7:2:11, Y_34"  "(7:2:11)'', Y_34" ..
              "int8([2 1 1.5])"  "int8(-4), [2 1 1.5]"  ..
              "int8(-4), int8([2 1 1.5])" ];
        for w = ["", ", []", ", 0.6"]             // bars width
            if grep(xy,",")==[] & w~=""
                continue
            end
            for col = ["", ", []", ", ""pink"""]      // colors
                if grep(xy,",")==[] & w=="" & col==", []"
                    continue
                end
                for sty = ["" ", ""stacked"""]        // style
                    mprintf("bar(%s%s%s%s%s)\n",a,xy,w,col,sty);
                    clf
                    ax = gca();
                    cmd = "bar("+a+xy+w+col+sty+")";
                    assert_checkequal(execstr(cmd, "errcatch"), 0);
                end
            end
        end
    end
end
// Multiple colors


// Expected Errors
// ---------------
msg = "bar: Wrong number of input argument(s): 1 to 6 expected."
assert_checkerror("clf, bar()", msg);
assert_checkerror("clf, bar(1,2,3,4,5,6,7)", msg);

// Bad handle type
msg = "bar: Argument #1: Graphic handle of type ''Axes'' expected."
assert_checkerror("clf, bar(gcf(),7, [3 -2 1]'')", msg);

// X, Y
msg = "bar: Argument #1: Decimal number(s) expected.";
assert_checkerror("clf, bar(''stacked'')", msg);
assert_checkerror("clf, bar([2 3]<1)", msg);
msg = "bar: Argument #2: Decimal number(s) expected.";
assert_checkerror("clf, bar(gca(), ''stacked'')", msg);
assert_checkerror("clf, bar(gca(), [2 3]<1)", msg);
msg = "bar: Argument #1: Non-empty matrix expected.";
assert_checkerror("clf, bar([])", msg);
msg = "bar: Argument #2: Non-empty matrix expected.";
assert_checkerror("clf, bar(gca(), [])", msg);
msg = "bar: Argument #2: Non-empty matrix expected.";
assert_checkerror("clf, bar([2 3],[])", msg);
msg = "bar: Argument #3: Non-empty matrix expected.";
assert_checkerror("clf, bar(gca(), [2 3],[])", msg);
msg = "bar: Argument #3: Non-empty matrix expected.";
assert_checkerror("clf, bar(gca(), [2 3],[])", msg);
msg = "bar: Argument #2: Wrong color specification.";
assert_checkerror("clf, bar(3,[1 2]<3)", msg);

msg = "bar: Arguments #1 and #2: Incompatible sizes."
assert_checkerror("clf, bar(7, [3 -2 1]'')", msg);
msg = "bar: Arguments #2 and #3: Incompatible sizes."
assert_checkerror("clf, bar(gca(), 7, [3 -2 1]'')", msg);

// Width
msg = "bar: Argument #3: Must be in the interval [0, 1]."
assert_checkerror("clf, bar(7, [3 -2 1], -0.5)", msg);
assert_checkerror("clf, bar(7, [3 -2 1], 1.1)", msg);
msg = "bar: Argument #3: Scalar (1 element) expected.";
assert_checkerror("clf, bar(7, [3 -2 1], [0.3 0.4])", msg);

msg = "bar: Argument #4: Must be in the interval [0, 1]."
assert_checkerror("clf, bar(gca(), 7, [3 -2 1], -0.5)", msg);
assert_checkerror("clf, bar(gca(), 7, [3 -2 1], 1.1)", msg);
msg = "bar: Argument #4: Scalar (1 element) expected.";
assert_checkerror("clf, bar(gca(), 7, [3 -2 1], [0.3 0.4])", msg);

// Colors
msg = "bar: Argument #3: Wrong color specification.";
assert_checkerror("clf, bar(7, [3 -2 1],[""abc"" ""red""])", msg);
msg = "bar: Argument #4: Wrong color specification.";
assert_checkerror("clf, bar(gca(), 7, [3 -2 1],[""abc"" ""red""])", msg);
// # of colors =/= number of categories
msg = "bar: Arguments #2 and #3: Incompatible sizes.";
assert_checkerror("clf, bar(7, [3 -2 1],[""green"" ""red""])", msg);
msg = "bar: Arguments #3 and #4: Incompatible sizes.";
assert_checkerror("clf, bar(gca(), 7, [3 -2 1],[""green"" ""red""])", msg);

// Style
msg = "bar: Argument #3: Scalar (1 element) expected.";
assert_checkerror("clf, bar(7, [3 -2 1],[""abc"" ""stacked""])", msg);
msg = "bar: Argument #4: Scalar (1 element) expected.";
assert_checkerror("clf, bar(gca(), 7, [3 -2 1],[""abc"" ""stacked""])", msg);

