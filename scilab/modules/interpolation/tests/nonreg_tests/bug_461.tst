// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 461 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=461
//
// <-- Short Description -->
//    Bug Report Id: 11985200361815216
//    Result  of following operations:
//    size(s0)
//     ans  =
//    !   1.    43. !
//
//    size(s1)   // and for s2 and s3
//     ans  =
//    !   43.    1. !
//    s0 is vector-row and s1, s2 s3- vector-colum
//
//
//    On Scilab 2.7 with " interp " function
//    The Error Messages are:
//                      !--error     5
//    inconsistent column/row dimensions
//    Commands:  x=[0. 1.01 2.01 3. 4.02];
//    f=[0. 1. 3.9 8.75 16.5];
//    plot(x,f);
//    d=splin(x,f );
// ...

correct=%F;
x=[0. 1.01 2.01 3. 4.02]; 
f=[0. 1. 3.9 8.75 16.5]; 
plot(x,f); 
d=splin(x,f );
xx=0:0.1:4.2;
[s0,s1,s2,s3]=interp(xx,x,f,d);
s0=s0';
s1=s1';
s2=s2';
s3=s3';
result=execstr("plot2d(xx,[s0 s1 s2 s3])","errcatch","n");
xdel();

if result<>0 then pause,end
