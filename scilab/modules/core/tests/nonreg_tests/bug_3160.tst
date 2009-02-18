// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3160 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3160
//
// <-- Short Description -->
// Missing variable in called function

// <-- TEST WITH GRAPHIC -->

function vt()
  ap(1,2,3,4)
  ap(1,2,3,4)
endfunction

function ap(v1,v2,v3,v4)
  [lhs,rhs]=argn()
  mprintf("rhs=%d\n",rhs)
  mprintf("v1 = %d\tv2 = %d\tv3 = %d\tv4 = %d\n",v1,v2,v3,v4)
  for i=1:1
    plot2d(1:10,1:10,rect=[0,0,11,11])
  end
endfunction

if execstr("vt();","errcatch")<>0 then pause;end
