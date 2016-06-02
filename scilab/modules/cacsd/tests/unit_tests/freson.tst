//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
h=syslin("c",-1+%s,(3+2*%s+%s^2)*(50+0.1*%s+%s^2));
fr=freson(h);
assert_checkalmostequal(fr,[0.26162097704;1.12527981797]);


hd=dscr(h,0.05);
fr=freson(hd);
assert_checkalmostequal(fr,[0.26155702493;1.12527923264]);
