// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8549 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8549
//
// <-- Short Description -->
// The neldermead function does not help to debug the cost function.
//

function [ f , index ] = myquad ( x , index )
  f = x(1)^2 + x(2)^2 + x(3)^2
endfunction
x0 = [1.2 1.9].';
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",myquad);
nm = nmplot_configure(nm,"-x0",x0);
instr = "nm = nmplot_search(nm)";
lclmsg = gettext ( "%s: Cannot evaluate cost function with ""%s"": %s" );
scimsg = msprintf(_("Invalid index.\n"));
assert_checkerror(instr, lclmsg , [], "optimbase_checkcostfun", "[f,index]=costf(x0,1)", scimsg);
nm = nmplot_destroy(nm);
