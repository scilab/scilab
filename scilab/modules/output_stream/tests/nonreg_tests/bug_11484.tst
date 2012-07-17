// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 11484 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11484
//
// <-- Short Description -->
// wrong display for lists with elements whose display is overloaded
//
x = struct('a',4,'b',3) 
y = struct('a',5,'b',6) 
z = struct('a',7,'b',2) 
list(list(z),list(x,y))

function %foo_p(l), mprintf("foo %d\n",l(2));endfunction
x=tlist('foo',4);y=tlist('foo',5);z=tlist('foo',6);
l=list(list(z),1,list(x,y))
