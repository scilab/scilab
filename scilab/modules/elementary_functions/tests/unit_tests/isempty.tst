// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2016, 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for isempty function
// =============================================================================

// empty matrix
assert_checktrue(isempty([]));

// character string
assert_checktrue(isempty(''));

// matrix of character string
assert_checktrue(isempty([ '' , '' ; '' , '' ]));
assert_checkfalse(isempty([ ' ' , '' ; '' , '' ]));

// empty list
assert_checktrue(isempty(list()));

// list with only undefined elements
assert_checktrue(isempty(list(,)));

// empty structure()
assert_checktrue(isempty(struct()));

// empty cell()
assert_checktrue(isempty({}));

// nested empty containers
assert_checktrue(isempty(list({}, list("",,[]), [], struct())));
s.r = [];
s.c = {};
s.L = list();
s.s = struct();
assert_checktrue(isempty(s));

// existing overloads
t = tlist("abc");
function r = %abc_isempty(L)
    r = "%abc_isempty called"
endfunction
assert_checkequal(isempty(t), "%abc_isempty called");
t = mlist("abc");
assert_checkequal(isempty(t), "%abc_isempty called");

// other mlists are never empty
assert_checkfalse(isempty(mlist('')));
