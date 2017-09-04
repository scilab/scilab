// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14573 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14573
//
// <-- Short Description -->
// varargout error when it is not alone

//varargout alone
function [varargout]=foo0(varargin)
    for i=1:argn(2)
        varargout(i)=varargin(i);
    end
endfunction

[a,b]=foo0(1,2,3,4,5);
assert_checkequal(a, 1);
assert_checkequal(b, 2);

[a,b,c]=foo0(1,2,3,4,5);
assert_checkequal(a, 1);
assert_checkequal(b, 2);
assert_checkequal(c, 3);

[a,b,c,d]=foo0(1,2,3,4,5);
assert_checkequal(a, 1);
assert_checkequal(b, 2);
assert_checkequal(c, 3);
assert_checkequal(d, 4);

[a,b,c,d,e]=foo0(1,2,3,4,5);
assert_checkequal(a, 1);
assert_checkequal(b, 2);
assert_checkequal(c, 3);
assert_checkequal(d, 4);
assert_checkequal(e, 5);

//1 output var + varargout
function [aa, varargout]=foo1(varargin)
    aa = argn(2);
    for i=1:aa
        varargout(i)=varargin(i);
    end
endfunction

[a,b]=foo1(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 1);

[a,b,c]=foo1(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 1);
assert_checkequal(c, 2);

[a,b,c,d]=foo1(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 1);
assert_checkequal(c, 2);
assert_checkequal(d, 3);

//2 output vars + varargin
function [aa, bb, varargout]=foo2(varargin)
    aa = argn(2);
    bb = argn(1);
    for i=1:aa
        varargout(i)=varargin(i);
    end
endfunction

[a,b]=foo2(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 2);

[a,b,c]=foo2(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 3);
assert_checkequal(c, 1);

[a,b,c,d]=foo2(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 4);
assert_checkequal(c, 1);
assert_checkequal(d, 2);

[a,b,c,d,e]=foo2(1,2,3);
assert_checkequal(a, 3);
assert_checkequal(b, 5);
assert_checkequal(c, 1);
assert_checkequal(d, 2);
assert_checkequal(e, 3);

