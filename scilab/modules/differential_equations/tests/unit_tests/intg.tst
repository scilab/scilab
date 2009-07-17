// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// to check that intg works
function y=f(x),y=x*sin(30*x)/sqrt(1-((x/(2*%pi))^2)),endfunction
exact=-2.5432596188;
I=intg(0,2*%pi,f);
if abs(exact-I) > 1 then pause,end