// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2435 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2435
//
// <-- Short Description -->
//    The following "optim based" function mades scilab hangs

function [f,g,ind]=test_cost(x,ind)
	xref=[1;2;3];
	f=0.5*norm(x-xref)^2
	g=x-xref;
endfunction

x0=[1;-1;1];

if execstr('[f,xopt]=optim(test_cost,,''b'',-2*ones(x0),2*ones(x0),x0);','errcatch') == 0  then pause,end
