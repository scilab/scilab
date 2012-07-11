// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// test des insertions dans les listes
a=1,b=3;c=2;d=[1 2 3];e=[3 4 5];f=[10;20];g=[44 55];
h=5;i=4;a0=10;c0=20;b0=30;g0=[44 55 66; 10 20 30];


l=list(a);if l(1)<>a then pause,end
l=list(a);if l(1)(1)<>a(1) then pause,end
l=list(a);if l(1)(1,1)<>a(1,1) then pause,end
l=list(a,d,b);if l(2)<>d then pause,end
l=list(a,d,b);if l(2)(1:2)<>d(1:2) then pause,end
l=list(a,d,b);if l(2)(1,2)<>d(1,2) then pause,end
//
l=list(a,list(b,c,d));if l(2)(3)<>d then pause,end
l=list(a,list(b,c,d));if l(2)(3)(3)<>d(3) then pause,end
l=list(a,list(b,c,d));if l(2)(3)([3 1])<>d([3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(1,[3 1])<>d(1,[3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(:,[3 1])<>d(:,[3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(:,[$ 1])<>d(:,[$ 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)($,[$ 1])<>d($,[$ 1]) then pause,end
//
l=list(a,list(b,c,d),h);if l(2)(3)<>d then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(3)<>d(3) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)([3 1])<>d([3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(1,[3 1])<>d(1,[3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(:,[3 1])<>d(:,[3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(:,[$ 1])<>d(:,[$ 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)($,[$ 1])<>d($,[$ 1]) then pause,end
//
l=list(a,1/%s);if l(2)<>1/%s then pause,end
l=list(a,1/%s);if l(2)('num')<>1 then pause,end
l=list(a,1/%s,g);if l(2)('den')(1,1)<>%s then pause,end
l=list(a,1/%s,g);if l(2)<>1/%s then pause,end
l=list(a,1/%s,g);if l(2)('num')<>1 then pause,end
l=list(a,1/%s,g);if l(2)('den')(1,1)<>%s then pause,end


a=1:10,b=3:2:33;c=(-10:0)';d=[1 2+%s 3];e=['1';'2';'3';'4'];
f=[%t %t %f];g=['12345','abcdefghijk'];
h=rand(3,3);i=eye(10,10);a0=10+%s;c0=20;b0=sparse(eye(30,30));

l=list(a);if l(1)<>a then pause,end
l=list(a);if l(1)(1)<>a(1) then pause,end
l=list(a);if l(1)(1,1)<>a(1,1) then pause,end
l=list(a,d,b);if l(2)<>d then pause,end
l=list(a,d,b);if l(2)(1:2)<>d(1:2) then pause,end
l=list(a,d,b);if l(2)(1,2)<>d(1,2) then pause,end
//
l=list(a,list(b,c,d));if l(2)(3)<>d then pause,end
l=list(a,list(b,c,d));if l(2)(3)(3)<>d(3) then pause,end
l=list(a,list(b,c,d));if l(2)(3)([3 1])<>d([3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(1,[3 1])<>d(1,[3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(:,[3 1])<>d(:,[3 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)(:,[$ 1])<>d(:,[$ 1]) then pause,end
l=list(a,list(b,c,d));if l(2)(3)($,[$ 1])<>d($,[$ 1]) then pause,end
//
l=list(a,list(b,c,d),h);if l(2)(3)<>d then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(3)<>d(3) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)([3 1])<>d([3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(1,[3 1])<>d(1,[3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(:,[3 1])<>d(:,[3 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)(:,[$ 1])<>d(:,[$ 1]) then pause,end
l=list(a,list(b,c,d),h);if l(2)(3)($,[$ 1])<>d($,[$ 1]) then pause,end
//
N=[1 2 3];
l=list(a,N/%s);if l(2)<>N/%s then pause,end
l=list(a,N/%s);if l(2)('num')<>N then pause,end
l=list(a,N/%s,g);if l(2)('den')(1,1)<>%s then pause,end
l=list(a,N/%s,g);if l(2)<>N/%s then pause,end
l=list(a,N/%s,g);if l(2)('num')<>N then pause,end
l=list(a,N/%s,g);if l(2)('den')(1,1)<>%s then pause,end

