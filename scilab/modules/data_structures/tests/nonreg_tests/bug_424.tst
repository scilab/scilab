//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 424 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=424
//
// <-- Short Description -->
//    hypermatrix insertion problem

if execstr("S=[];for k=1:5,S(1:2,1:3,k)=rand(2,3);end","errcatch")<>0 then pause; end

