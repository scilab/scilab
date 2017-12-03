// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

function foo_good(a,b)
    x = checkNamedArguments();
    assert_checktrue(isempty(x));
endfunction

foo_good(1);
foo_good(1, 2);
foo_good(a = 1, b = 2);
foo_good(b = 2, a = 2);

function foo_bad(a,b)
    x = checkNamedArguments();
    assert_checkfalse(isempty(x));
endfunction

foo_bad(a = 1, c = 2);
foo_bad(c = 2, b = 2);


function foo_varargin_good(a, b, varargin)
    x = checkNamedArguments();
   assert_checktrue(isempty(x));
endfunction

foo_varargin_good(1);
foo_varargin_good(1, 2);
foo_varargin_good(a = 1, b = 2);
foo_varargin_good(b = 2, a = 2);
foo_varargin_good(b = 2, a = 2, 3, 4);

function foo_varargin_bad(a, b, varargin)
    x = checkNamedArguments();
    assert_checkfalse(isempty(x));
endfunction

foo_varargin_bad(c = 2);
foo_varargin_bad(a = 1, c = 2);
foo_varargin_bad(c = 1, b = 2);
foo_varargin_bad(b = 2, c = 3, a = 3, 4);
foo_varargin_bad(c = 3, a = 1, b = 2, 4);
