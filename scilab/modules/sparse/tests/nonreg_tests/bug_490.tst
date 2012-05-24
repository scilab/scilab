// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 490 -->
// Please note that it is store under the request feature of Scilab
// 
// <-- Bugzilla URL -->
// http://requestzilla.scilab.org/show_bug.cgi?id=490
//
// <-- Short Description -->
//    Wrong size detection in sparse matrix

toto = spzeros(2^16, 2^16);
toto(1, 8) =toto(1, 8)+5;
toto(1, 8) =toto(1, 8)+5;
if toto(1, 8) == []; then pause, end

