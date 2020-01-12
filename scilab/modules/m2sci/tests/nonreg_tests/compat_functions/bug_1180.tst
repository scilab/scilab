//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1180 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1180
//
// <-- Short Description -->
//    mtlb_filter lines 
//
//    b=b*z^(max(degree(a)-degree(b),0))
//    a=a*z^(max(degree(b)-degree(a),0))
//
//    should be replaced by :
//
//    da = degree(a)
//    db = degree(b)
//    if (da-db) > 0 then b=b*z^(da-db); end 
//    if (db-da) > 0 then a=a*z^(db-da); end

x    = zeros(1:10);
x(5) = 1;
b    = [ 0 1 0 ]
y    = mtlb_filter(b,1,x);

if y<>[0,0,0,0,0,1,0,0,0,0] then pause,end
