//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
u=1:10;v=[1 -1];
assert_checkequal(conv(u,v),[ones(1,10) -10]);
assert_checkequal(conv(u,v,"full"),[ones(1,10) -10]);
assert_checkequal(conv(u,v'),[ones(1,10) -10]);
assert_checkequal(conv(u',v),[ones(10,1); -10]);

assert_checkequal(conv(u,v,"same"),[ones(1,9) -10]);
assert_checkequal(conv(v,u,"same"),[1 1]);
assert_checkequal(conv(u,v,"valid"),ones(1,9));
assert_checkequal(conv(v,u,"valid"),[]);
assert_checkequal(conv(u,v*%i),[ones(1,10) -10]*%i);
assert_checkequal(conv(u*%i,v*%i),-[ones(1,10) -10]+0*%i);
