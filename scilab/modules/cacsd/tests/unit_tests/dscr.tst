// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

// Case #1
a=-1/3;
b=1;
c=1;
sl=syslin('c',a,b,c);
sld=dscr(sl,0.1);
assert_close ( sld.dt , 0.1 , %eps );
computed = sld.A;
expected = exp(sl.A*0.1);
assert_close ( computed , expected , %eps );
computed = sld.B;
expected = sl.A\(sld.A-eye())*sl.B;
assert_close ( computed , expected , %eps );

// Case #2
a=[0.9,0,0.6,-1.4,-4.2;
   0.2,0.1,-0.2,0.5,0.6;
   -4.3,0,2.2,0,2.4;
   -3.7,-0.5,2.4,-0.6,2.7;
   6.4,0.1,-4,-0.5,-4];

b=[-0.1,-0.1,0;
   0,0,0.1;
   -0.1,0.2,-0.1;
   0.2,0.2,-0.6;
   0.2,-0.1,0.1];
 
c=[2,7,-2,5,1
   0,-1,3,0,2];
d=[1,0,0
   0,0,0];

sl=syslin('c',a,b,c,d);
sld=dscr(sl,0.1);
assert_close ( sld.dt , 0.1 , %eps );
computed = sld.A;
expected = expm(sl.A*0.1);
assert_close ( computed , expected , %eps );
computed = sld.B;
expected = sl.A\(sld.A-eye())*sl.B;
assert_close ( computed , expected , 1.e3 * %eps );

