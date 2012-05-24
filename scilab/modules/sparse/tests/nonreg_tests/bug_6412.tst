// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE <yann.collette@scilab.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6412 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6412
//
// <-- Short Description -->
// the function sp2adj was not working

A = sprand(100,50,.05);
[xadj,adjncy,anz]= sp2adj(A);
[n,m]=size(A);
p = adj2sp(xadj,adjncy,anz,[n,m]);
if norm(A-p)~=0 then pause, end
