// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2009 - INRIA - Michael Baudin, Antoine Elias
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2775 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2775
//
res_1 = 0.0 * %nan;
res_2 = %nan * 0.0;

if ~isnan(res_1) then pause,end
if ~isnan(res_2) then pause,end

// Full tests after reading from 
// "IEEE Standard for Floating-Point Arithmetic"
// IEEE Std 754?-2008
// 29 August 2008
// (Revision of IEEE Std 754-1985)

expectedadd = [
-%inf 	-%inf 	-%inf 	-%inf 	-%inf 	%nan 	%nan
-%inf 	-2 	-1 	-1 	0 	%inf 	%nan
-%inf 	-1 	-0 	0 	1 	%inf 	%nan
-%inf 	-1 	0 	0 	1 	%inf 	%nan
-%inf 	0 	1 	1 	2 	%inf 	%nan
%nan 	%inf 	%inf 	%inf 	%inf 	%inf 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expectedminus = [
%nan 	-%inf 	-%inf 	-%inf 	-%inf 	-%inf 	%nan
%inf 	0 	-1 	-1 	-2 	-%inf 	%nan
%inf 	1 	0 	-0 	-1 	-%inf 	%nan
%inf 	1 	0 	0 	-1 	-%inf 	%nan
%inf 	2 	1 	1 	0 	-%inf 	%nan
%inf 	%inf 	%inf 	%inf 	%inf 	%nan 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expectedmult = [
%inf 	%inf 	%nan 	%nan 	-%inf 	-%inf 	%nan
%inf 	1 	0 	-0 	-1 	-%inf 	%nan
%nan 	0 	0 	-0 	-0 	%nan 	%nan
%nan 	-0 	-0 	0 	0 	%nan 	%nan
-%inf 	-1 	-0 	0 	1 	%inf 	%nan
-%inf 	-%inf 	%nan 	%nan 	%inf 	%inf 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expecteddiv = [
%nan 	%inf 	%inf 	-%inf 	-%inf 	%nan 	%nan
0 	1 	%inf 	-%inf 	-1 	-0 	%nan
0 	0 	%nan 	%nan 	-0 	-0 	%nan
-0 	-0 	%nan 	%nan 	0 	0 	%nan
-0 	-1 	-%inf 	%inf 	1 	0 	%nan
%nan 	-%inf 	-%inf 	%inf 	%inf 	%nan 	%nan
%nan 	%nan 	%nan 	%nan 	%nan 	%nan 	%nan];

expectedsqrt = [
%i*%inf  
%i
0
0
1.
%inf
%nan];

expectedmodulo = [
%nan  %nan -%inf -%inf  %nan  %nan  %nan
-1.  0. -1. -1.  0. -1.  %nan
0.  0.  0.  0.  0.  0.  %nan
0.  0.  0.  0.  0.  0.  %nan
1.  0.  1.  1.  0.  1.  %nan
%nan  %nan  %inf  %inf  %nan  %nan  %nan
%nan  %nan  %nan  %nan  %nan  %nan  %nan];

left=[-%inf,-1,-0,+0,1,%inf,%nan];
right=left;
n=size(left,2);

computedadd = zeros(n,n);
computedminus = zeros(n,n);
computedmult = zeros(n,n);
computeddiv = zeros(n,n);
computedsqrt = zeros(n);
computedsqrt = zeros(n);
computedmodulo = zeros(n,n);
ieee(2)
for i=1:n
for j=1:n
  l=left(i);
  r=right(j);
  computedadd (i,j)=l+r;
  computedminus (i,j)=l-r;
  computedmult (i,j)=l*r;
  computeddiv (i,j)=l/r;
  computedsqrt (i) = sqrt(l);
  computedmodulo (i,j) = modulo(l , r);
end
end

if ~and(computedadd <> expectedadd) then pause,end
if ~and(computedminus <> expectedminus) then pause,end
if ~and(computedmult <> expectedmult) then pause,end
if ~and(computeddiv <> expecteddiv) then pause,end
if ~and(computedsqrt <> expectedsqrt) then pause,end
if ~and(computedmodulo <> expectedmodulo) then pause,end

