// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

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

// mlist is never empty
assert_checkfalse(isempty(mlist('')));
