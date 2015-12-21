//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdffnc() function
// =============================================================================

// Run with test_run('statistics','cdffnc',['no_check_error_output']);

prec     = 1.e-5;

f        = [1:2];
dfn      = [1:2];
dfd      = 2*dfn;
pn       = [0,1];

[P,Q]    = cdffnc("PQ",f,dfn,dfd,pn);
[f1]     = cdffnc("F",dfn,dfd,pn,P,Q);
[dfn1]   = cdffnc("Dfn",dfd,pn,P,Q,f);
//[dfd1] = cdffnc("Dfd",pn,P,Q,f,dfn);
[pnonc]  = cdffnc("Pnonc",P,Q,f,dfn,dfd);

if norm(f1-f)       > prec then pause,end
if norm(dfn1-dfn)   > prec then pause,end
//if norm(dfd1-dfd) > prec then pause,end
if norm(pnonc-pn)   > prec then pause,end

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
dfn      = 1;
dfd      = 2;
pn       = 0;

f        = %inf; // Inf
[P,Q]    = cdffnc("PQ", f, dfn, dfd, pn);
assert_checkequal(P, 1);
assert_checkequal(Q, 0);

f        = %nan; // NaN
[P,Q]    = cdffnc("PQ", f, dfn, dfd, pn);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
