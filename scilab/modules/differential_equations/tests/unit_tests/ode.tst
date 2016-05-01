// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
warning("off")
ilib_verbose(0);

version = getversion("scilab");

// to check that ode works
// ---------- Simple one dimension ODE (Scilab function external)
// dy/dt=y^2-y sin(t)+cos(t), y(0)=0
function ydot=f(t,y),ydot=y^2-y*sin(t)+cos(t),endfunction
y0=0;t0=0;t=0:0.1:%pi;
y=ode(y0,t0,t,f);
assert_checkalmostequal(size(y), [1 32] , %eps, [], "matrix");
clear y;
clear t;
//*************************** function F and lsoda ********************************/
// create functions
cd TMPDIR;

CC=["void fex1(int* neq, double* t, double* y, double* ydot)"
"{"
"   ydot[0] = -0.04*y[0] + 1.0e+4*y[1]*y[2];"
"   ydot[2] = 3.0e+7*y[1]*y[1];"
"   ydot[1] = -ydot[0] - ydot[2];"
"}"];
mputl(CC,TMPDIR+"/fex1.c");
ilib_for_link("fex1","fex1.c",[],"c");
exec loader.sce;

C=[ "void fex2(int* neq, double* t, double* y, double* ydot)"
"{"
"   ydot[0] = y[4]*y[0] + y[5]*y[1]*y[2];"
"   ydot[2] = y[3]*y[1]*y[1];"
"   ydot[1] = -ydot[0] - ydot[2];"
"}"];

mputl(C,TMPDIR+"/fex2.c");
ilib_for_link("fex2","fex2.c",[],"c");
exec loader.sce;
clear f;
function ydot = f(t,yin)
    ydot(1)=-0.040*yin(1) + 1.0D4*yin(2)*yin(3);
    ydot(3)=3.0D7*yin(2)**2;
    ydot(2)=-ydot(1) - ydot(3);
endfunction

function ydot = f1(t,yin,a,b,c)
    ydot(1)=b*yin(1) + c*yin(2)*yin(3);
    ydot(3)=a*yin(2)**2;
    ydot(2)=-ydot(1) - ydot(3);
endfunction

function ydot = f2(t,yin,a)
    ydot(1)=a(2)*yin(1) + a(3)*yin(2)*yin(3);
    ydot(3)=a(1)*yin(2)**2;
    ydot(2)=-ydot(1) - ydot(3);
endfunction

// init variables
y(1)    = 1;
y(2)    = 0;
y(3)    = 0;
t       = 0;
tout    = 0.4*exp((0:11)*log(10));
rtol    = 1.0d-4;
atol(1) = 1.0d-6;
atol(2) = 1.0d-10;
atol(3) = 1.0d-6;

// result provide by lsoda documentation.
// on a cdc-7600 in single precision.
//   at t =  4.0000e-01
resDoc(:,1) = [ 9.851712e-01 ; 3.386380e-05 ; 1.479493e-02 ];
//   at t =  4.0000e+00
resDoc(:,2) = [ 9.055333e-01 ; 2.240655e-05 ; 9.444430e-02 ];
//   at t =  4.0000e+01
resDoc(:,3) = [ 7.158403e-01 ; 9.186334e-06 ; 2.841505e-01 ];
//   at t =  4.0000e+02
resDoc(:,4) = [ 4.505250e-01 ; 3.222964e-06 ; 5.494717e-01 ];
//   at t =  4.0000e+03
resDoc(:,5) = [ 1.831975e-01 ; 8.941774e-07 ; 8.168016e-01 ];
//   at t =  4.0000e+04
resDoc(:,6) = [ 3.898730e-02 ; 1.621940e-07 ; 9.610125e-01 ];
//   at t =  4.0000e+05
resDoc(:,7) = [ 4.936363e-03 ; 1.984221e-08 ; 9.950636e-01 ];
//   at t =  4.0000e+06
resDoc(:,8) = [ 5.161831e-04 ; 2.065786e-09 ; 9.994838e-01 ];
//   at t =  4.0000e+07
resDoc(:,9) = [ 5.179817e-05 ; 2.072032e-10 ; 9.999482e-01 ];
//   at t =  4.0000e+08
resDoc(:,10) = [ 5.283401e-06 ; 2.113371e-11 ; 9.999947e-01 ];
//   at t =  4.0000e+09
resDoc(:,11) = [ 4.659031e-07 ; 1.863613e-12 ; 9.999995e-01 ];
//   at t =  4.0000e+10
resDoc(:,12) = [ 1.404280e-08 ; 5.617126e-14 ; 1.000000e+00 ];

// f as a string (dynamic link function)
res  = ode(y, t,tout, rtol, atol, "fex1");
// f as a list(string,...
if version(1) > 5 then
    res2 = ode(y, t, tout, rtol, atol, list("fex2", 3.0d+7, -0.04, 1.0d+4));
end
// f as a macro
res3 = ode(y, t, tout, rtol, atol, f);
// f as a list(macro,...
res4 = ode(y, t, tout, rtol, atol, list(f1, 3.0d+7, -0.04, 1.0d+4));
args = [ 3.0d+7, -0.04, 1.0d+4 ];
res5 = ode(y, t, tout, rtol, atol, list(f2, args));
// f as a string (static link function)
res6 = ode(y, t,tout, rtol, atol, "fex");

// check results
assert_checkalmostequal(resDoc, res, 2d-7, [], "matrix"); // There are a little diff between resDoc and res
if version(1) > 5 then
    assert_checkalmostequal(res, res2, 2d-7, [], "matrix"); // because results provides by lsoda
end
assert_checkalmostequal(res, res3, 2d-7, [], "matrix"); // documentation are in single precision.
assert_checkalmostequal(res, res4, 2d-7, [], "matrix");
assert_checkalmostequal(res, res5, 2d-7, [], "matrix");
assert_checkalmostequal(res, res6, 2d-7, [], "matrix");

//*************************** w iw ********************************/
tout2 = 0.4*exp(12*log(10));
tout3 = 0.4*exp((0:12)*log(10));
[yout w iw] = ode(y, t, tout, rtol, atol, f);
yout1 = ode(y, t, tout2, rtol, atol, f, w, iw);
yout2 = ode(y, t, tout3, rtol, atol, f);

assert_checkalmostequal(yout2(3*12+1:3*13), yout1, %eps, [], "matrix");

//*************************** Polynom ********************************/
//y(1) = 1;
//y(2) = 2;
//y(3) = 3;
//yy   = 1+2*%s+3*%s*%s;

//res  = ode(y, t,tout, rtol, atol, 'fex1');

//res6  = ode(yy, t, tout, rtol, atol, 'fex1');
//for i=1:12, assert_checkalmostequal(poly(res(:,i), "s", "coeff"),res6(i), %eps); end
//res7 = ode(yy, t, tout, rtol, atol, list('fex2', 3.0d+7, -0.04, 1.0d+4));
//for i=1:12, assert_checkequal(poly(res(:,i), "s", "coeff"), res7(i), %eps); end

//*************************** function Jac and lsode ********************************/
CC=["void jac(int* neq, double* t, double* y, int* mu, int* ml, double* j, int nj)"
"{"
"  j[0] = y[6]; "
"  j[1] = y[7]; "
"  j[2] = y[8]; "
"  j[3] = y[9]; "
"}"];

mputl(CC,TMPDIR+"/jac.c");
ilib_for_link("jac","jac.c",[],"c");
exec loader.sce;

CC=["void jac2(int* neq, double* t, double* y, int* mu, int* ml, double* j, int nj)"
"{"
"  j[0] = 10; "
"  j[1] = 0; "
"  j[2] = 0; "
"  j[3] = -1; "
"}"];
mputl(CC,TMPDIR+"/jac2.c");
ilib_for_link("jac2","jac2.c",[],"c");
exec loader.sce;

// ydot = A * y
C=[ "void fext(int* neq, double* t, double* y, double* ydot)"
"{"
"   ydot[0] = y[2]*y[0] + y[4]*y[1];"
"   ydot[1] = y[3]*y[0] + y[5]*y[1];"
"}"];

mputl(C,TMPDIR+"/fext.c");
ilib_for_link("fext","fext.c",[],"c");
exec loader.sce;

C=[ "void fext2(int* neq, double* t, double* y, double* ydot)"
"{"
"   ydot[0] = 10*y[0] + 0*y[1];"
"   ydot[1] = 0*y[0] + (-1)*y[1];"
"}"];

mputl(C,TMPDIR+"/fext2.c");
ilib_for_link("fext2","fext2.c",[],"c");
exec loader.sce;
clear f;

function ydot=f(t, y)
    ydot=A*y
endfunction

function J=Jacobian(t, y)
    J=A
endfunction

A=[10,0;0,-1];
y0=[0;1];
t0=0;
t=1;

res  = ode("stiff", y0, t0, t, f, Jacobian);
if version(1) > 5 then
    res1 = ode("stiff", y0, t0, t, list("fext", 10, 0, 0,-1), list("jac", A));
end
res2 = ode("stiff", y0, t0, t, "fext2", "jac2");
res3 = ode("stiff", y0, t0, t, f, "jac2");
res4 = ode("stiff", y0, t0, t, "fext2", Jacobian);

assert_checkalmostequal(res, expm(A*t)*y0, 1.0D-7, [], "matrix");
if version(1) > 5 then
    assert_checkalmostequal(res, res1, %eps, [], "matrix");
end
assert_checkalmostequal(res, res2, %eps, [], "matrix");
assert_checkalmostequal(res, res3, %eps, [], "matrix");
assert_checkalmostequal(res, res4, %eps, [], "matrix");

//*************************** discrete ********************************/
function yp=a_function(k,y)
    yp=A*y+B*u(k);
endfunction

y1 = [1;2;3];
A  = diag([0.2,0.5,0.9]);
B  = [1;1;1];
u  = 1:10;
n  = 5;

y =ode("discrete", y1, 1, 1:n, a_function);
for i = 1:4, y1(:,i+1) = A * y1(:,i) + B * u(i); end
assert_checkalmostequal(y, y1, %eps, [], "matrix");

// Now y evaluates  at [y3,y5,y7,y9]
y1 = [1;2;3];
t  = 3:2:9;
y  = ode("discrete", y1, 1, t, a_function);
for i=1:9, y1(:,i+1) = A * y1(:,i) + B * u(i); end
y1 = y1(:,t);
assert_checkalmostequal(y, y1, %eps, [], "matrix");

//*************************** root ********************************/
y0=1;
ng=1;

C=[ "void fextern(int* neq, double* t, double* y, double* ydot)"
"{"
"int i = 0;"
"for( i = 0; i < *neq; i++)"
"   ydot[i] = y[i];"
"}"];

mputl(C,TMPDIR+"/fextern.c");
ilib_for_link("fextern","fextern.c",[],"c");
exec loader.sce;
clear f;
function ydot=f(t,y)
    ydot=y;
endfunction

clear g;
// check rd result
function z=g(t,y)
    z=y-2;
endfunction
[y,rd]=ode("root",y0,0,2,f,ng,g);
assert_checkequal( rd(2) <> 1 , %f);

clear g;
function z=g(t,y)
    z=y-[2;2;33];
endfunction
[y,rd]=ode("root",y0,0,2,f,3,g);
assert_checkequal( rd(2) <> 1 , %f);
assert_checkequal( rd(3) <> 2 , %f);

clear g;
function z=g(t,y)
    z=y-[2;2;2];
endfunction
[y,rd]=ode("root",y0,0,2,f,3,g);
assert_checkequal( rd(2) <> 1 , %f);
assert_checkequal( rd(3) <> 2 , %f);
assert_checkequal( rd(4) <> 3 , %f);

clear g;
function z=g(t,y)
    z=y-[2;6;2;2];
endfunction
[y,rd]=ode("root",y0,0,2,f,4,g);
assert_checkequal( rd(2) <> 1 , %f);
assert_checkequal( rd(3) <> 3 , %f);
assert_checkequal( rd(4) <> 4 , %f);

// check y result

// result provide by lsodar documentation.
// on a cdc-7600 in single precision.
//   at t =  2.6400e-01
resDocRoot(:,1) = [ 9.899653e-01 ; 3.470563e-05 ; 1.000000e-02 ];
//        the above line is a root,  jroot =    0    1
//   at t =  4.0000e-01
resDoc(:,1) = [ 9.851712e-01 ; 3.386380e-05 ; 1.479493e-02 ];
//   at t =  4.0000e+00
resDoc(:,2) = [ 9.055333e-01 ; 2.240655e-05 ; 9.444430e-02 ];
//   at t =  4.0000e+01
resDoc(:,3) = [ 7.158403e-01 ; 9.186334e-06 ; 2.841505e-01 ];
//   at t =  4.0000e+02
resDoc(:,4) = [ 4.505250e-01 ; 3.222964e-06 ; 5.494717e-01 ];
//   at t =  4.0000e+03
resDoc(:,5) = [ 1.831975e-01 ; 8.941774e-07 ; 8.168016e-01 ];
//   at t =  4.0000e+04
resDoc(:,6) = [ 3.898730e-02 ; 1.621940e-07 ; 9.610125e-01 ];
//   at t =  4.0000e+05
resDoc(:,7) = [ 4.936363e-03 ; 1.984221e-08 ; 9.950636e-01 ];
//   at t =  4.0000e+06
resDoc(:,8) = [ 5.161831e-04 ; 2.065786e-09 ; 9.994838e-01 ];
//   at t =  2.0745e+07
resDocRoot(:,2) = [ 1.000000e-04 ; 4.000395e-10 ; 9.999000e-01 ];
//        the above line is a root,  jroot =    1    0
//   at t =  4.0000e+07
resDoc(:,9) = [ 5.179817e-05 ; 2.072032e-10 ; 9.999482e-01 ];
//   at t =  4.0000e+08
resDoc(:,10) = [ 5.283401e-06 ; 2.113371e-11 ; 9.999947e-01 ];
//   at t =  4.0000e+09
resDoc(:,11) = [ 4.659031e-07 ; 1.863613e-12 ; 9.999995e-01 ];
//   at t =  4.0000e+10
resDoc(:,12) = [ 1.404280e-08 ; 5.617126e-14 ; 1.000000e+00 ];

G=[ "void gex(int* neq, double* t, double* y, int* ng, double* gout)"
"{"
"gout[0] = y[0] - 1.0e-4;"
"gout[1] = y[2] - 1.0e-2;"
"}"];

mputl(G,TMPDIR+"/gex.c");
ilib_for_link("gex","gex.c",[],"c");
exec loader.sce;

y(1) = 1;
y(2) = 0;
y(3) = 0;
t0   = 0;

[yout1,rd1,w,iw] = ode("root", y, t0, tout, "fex1", 2, "gex");
assert_checkalmostequal(rd1(1), 2.64d-01, 1d-4);
[yout2,rd2,w,iw] = ode("root", y, t0, tout, "fex1", 2, "gex", w, iw);
assert_checkalmostequal(rd2(1), 2.0795776d+07, 4d-5);
err = execstr("[yout3,rd,w,iw] = ode(""root"", y, t0, tout, ""fex1"", 2, ""gex"", w, iw);","errcatch");
assert_checkequal( err == 0 , %f);

// check results
assert_checkalmostequal(resDocRoot(:,1), yout1, 2.0D-8, [], "matrix");
assert_checkalmostequal(resDocRoot(:,2), yout2(:,9), 2.0D-8, [], "matrix");
assert_checkalmostequal(resDoc(:,1:8), yout2(:,1:8), 2.0D-4, [], "matrix");

//*************************** rk/rkf/fix ********************************/
function ydot=functionF(t, y)
    ydot=y^2-y*sin(t)+cos(t)
endfunction

y0 = 0;
t0 = 0;
t  = 0:0.1:%pi;

rk   = ode("rk",  y0, t0, t, functionF);
rkf  = ode("rkf", y0, t0, t, functionF);
fixx = ode("fix", y0, t0, t, functionF);

rkRes = [0.    0.09983341664683527    0.19866933079512300    0.29552020666153711 0.38941834230905709    0.47942553860493514    0.56464247339623352    0.64421768723947215 0.71735609090195429    0.78332690963060869    0.8414709848117728     0.89120736006615475 0.93203908597292051    0.96355818542403104    0.98544972999664104    0.99749498661376];
rkRes(17:32) = [0.99957360305287668    0.99166481046564392    0.97384763089330029    0.94630008770455387 0.90929742684492987    0.86320936667026738    0.80849640384312127    0.74570521220233155 0.67546318057873300    0.59847214413334937    0.51550137185246248    0.42737988026620155 0.33498815018939587    0.23924932924832210    0.14112000809477554    0.04158066246848785];

rkfRes = [0.    0.09983341667063099    0.19866933087782307    0.2955202068043328    0.38941834246046680     0.47942553864900261    0.56464247314940919    0.64421768645637856    0.71735608928892303      0.78332690686480611    0.84147098056308622    0.89120735401875306    0.93203907784361117      0.96355817497519225    0.98544971704249074    0.99749497101988072    0.99957358472991464];
rkfRes(18:32) = [    0.99166478935902302    0.97384760697150774    0.94630006094857066    0.90929739724116376     0.86320933420871493    0.80849636852157181    0.74570517403636893    0.67546313961625104      0.59847210047141619    0.51550132565379336    0.42737983177229999    0.33498809972769594      0.23924927723128114    0.14111995500988161    0.04158060885936282];

assert_checkalmostequal(rkRes, rk, %eps * 20, [], "matrix");
assert_checkalmostequal(rkfRes, rkf, %eps, [], "matrix");
assert_checkalmostequal(rkf, fixx, %eps, [], "matrix");
warning("on")
