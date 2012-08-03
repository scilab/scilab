// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6745 -->
// <-- CLI SHELL MODE -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6745
//
// <-- Short Description -->
//  g_margin() returned an erroneous result

z=poly(0,'z');
Hbo=syslin(1,(0.042-0.03933*z-0.15407*z^2+0.18518*z^3+6.939e-18*z^4)/(-0.042+0.088*z+0.114*z^2-0.36*z^3+0.2*z^4));
[m,fr]=g_margin(Hbo);
if abs(fr-0.5)>1d-10 then pause,end
r=repfreq(Hbo,fr);
if ~isreal(r,1e-10) then pause,end
if real(r)>0 then pause,end
if abs(20*log10(1/abs(r))-m)>1d-10 then pause,end
