// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2009 - INRIA - Michael Baudin, Antoine Elias
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 2775 -->
//
// <-- JVM NOT MANDATORY -->
// <-- Bugzilla URL -->
// <-- NOT FIXED -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2775
//


//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if ( type(computed) == 1 & type(expected) == 1 ) then
    csize = size (computed,"*")
    esize = size (expected,"*")
    [cnonan , cnumb] = thrownan(computed)
    [enonan , enumb] = thrownan(expected)
    if ( and(enonan == cnonan) & and(enumb == cnumb) ) then
      flag = 1;
    else
      flag = 0;
    end
  else
    if computed==expected then
      flag = 1;
    else
      flag = 0;
    end
  end
  if flag <> 1 then pause,end
endfunction

// Bibliography :
// "IEEE Standard for Floating-Point Arithmetic"
// IEEE Std 754-2008
// 29 August 2008
// (Revision of IEEE Std 754-1985)

res_1 = 0.0 * %nan;
res_2 = %nan * 0.0;

assert_equal ( res_1 , %nan );
assert_equal ( res_2 , %nan );

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
  l=left(i);
  computedsqrt (i) = sqrt(l);
  for j=1:n
    r=right(j);
    computedadd (i,j)=l+r;
    computedminus (i,j)=l-r;
    computedmult (i,j)=l*r;
    computeddiv (i,j)=l/r;
    computedmodulo (i,j) = modulo(l , r);
  end
end

assert_equal ( computedadd    , expectedadd );
assert_equal ( computedminus  , expectedminus );
assert_equal ( computedmult   , expectedmult );
assert_equal ( computeddiv    , expecteddiv );
assert_equal ( computedsqrt   , expectedsqrt );
assert_equal ( computedmodulo , expectedmodulo );
