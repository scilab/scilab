// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6401 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6401
//
// <-- Short Description -->
// chfact() returned a error

a = [1,0,0;
0,1,0;
0,0,1];

aa = sparse(a);
ierr = execstr('chfact(aa)','errcatch');
if ierr <> 0 then pause,end

xadj= [ 1.  ;    1.  ;    1.  ;    1.  ];
adjncy = [];
neqns = 3.  ;
ierr = execstr('[perm,invp,nofsub]=ordmmd(xadj,adjncy,neqns)','errcatch');
if ierr <> 0 then pause,end

refperm  = [   3.  ;    2.  ;    1.  ];
refinvp  = [ 3.  ;    2.  ;    1.  ];
refnofsub  = 0;
if refperm <> perm then pause,end
if refinvp <> invp then pause,end
if refnofsub <> nofsub then pause,end


