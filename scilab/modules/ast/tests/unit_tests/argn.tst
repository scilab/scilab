// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->

// In the below functions
// "expected_n_lhs" is the number of expected lhs
// as seen by the function in argn(1) value

function varargout = lhs_test(expected_n_lhs)
    assert_checkequal(argn(1), expected_n_lhs)
    if argn(1)==1
        varargout(1) = 1:10
    end
end

function varargout = lhs_test_struct(expected_n_lhs)
    assert_checkequal(argn(1), expected_n_lhs)
    if argn(1)==1
        varargout(1) = struct("a",1,"b",2)
    end
end

function varargout = new_plot(expected_n_lhs, varargin)
    assert_checkequal(argn(1), expected_n_lhs)
    clf
    plot(varargin(:))
    if argn(1) == 1
        varargout(1) = gce()
    end
end

// argn(1) == 0 when there is no explicit or implicit lhs

lhs_test(0)
lhs_test_struct(0)
new_plot(0, 1:10, sin(1:10))

// argn(1) == 1 when there is an explicit lhs

x = lhs_test(1)
s = lhs_test_struct(1)
h = new_plot(1, 1:10, sin(1:10))

// argn(1) == 1 when there is no explicit lhs but the function call
// is used in expression or as an input argument
// in another function call

- lhs_test(1)
+ lhs_test(1)
1 + lhs_test(1)
cos(lhs_test(1))
fieldnames(lhs_test_struct(1))
set(new_plot(1, 1:10, sin(1:10)),"visible","off")

// argn(1) == 1 when there is no explicit lhs but recursive extraction
// is used on the result of function call

lhs_test(1)($) + lhs_test(1)(1)
lhs_test_struct(1).a + lhs_test_struct(1).b
new_plot(1, 1:10, sin(1:10)).visible

// argn(1) == 1 when there is no explicit lhs but  insertion
// is used on the result of function call, typically of graphic handle type

//new_plot(1, 1:10, sin(1:10)).visible = "on"

