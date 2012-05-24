// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7714 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id= 7714
//
// <-- Short Description -->
// diag, triu, tril applyed to rational matrices may change the formal variable name


h=[1/%s,1/%s;1/%s,1];

if or(triu(h)<>[1/%s,1/%s;0,1]) then pause,end
if or(tril(h)<>[1/%s,0;1/%s,1]) then pause,end
if or(diag(h)<>[1/%s;1]) then pause,end
if or(diag([1/%s;1])<>[1/%s,0;0,1]) then pause,end
