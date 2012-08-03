// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 670 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=670
//
// <-- Short Description -->
//    Stack problem in FUN2STRING. Occurs in many different 
//    situations, one situation can be reproduced using the code 
//    below.

deff('a = foo()', ...
		'b = tlist([''b''; ''c''], 0); ' + ...
		'a = tlist([''a''; ''b''], b); ' + ...
		'a.b.c = getText() + ''text 2'' ');

t=fun2string(foo);

t_ref=["function a=ans()";
"  b = tlist([''b'';''c''], 0);a = tlist([''a'';''b''], b);a.b.c = getText() + ''text 2''";
"endfunction"];

if or( t <> t_ref ) then pause,end
