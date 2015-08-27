//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9783 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9783
//
// <-- Short Description -->
// Insertion overloading does not seem to work with mlist


// create structure 
function [q] = new(x)
  q = mlist(['X','x'], x);
endfunction

// q2(I) = q1
function [q2] = %X_i_X(I,q1,q2)
  if (I == []) 
    return; 
  else
    x2 = q2.x;
    x1 = q1.x; 
    x2(I) = x1;
    q2 = new(x2);
  end
endfunction

q0 = new([])
q1 = new([91,93])
q2 = new([10,20,30,40])

q2([1,3]) = q1;
q2_ref = new([91 20 93 40]);
assert_checkequal(q2,q2_ref);

q2([]) = q0;
assert_checktrue(execstr("q2([])=q0","errcatch")==0);
