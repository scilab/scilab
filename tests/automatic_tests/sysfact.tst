getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sysfact_data.ref','rb');
//Kalman filter
Sys = ssrand(3, 2, 4);Sys('D') = rand(3, 2);
S = sysfact(Sys, lqr(Sys), 'post');
ww = minss(Sys * S);
%ans = ss2tf(gtild(ww) * ww);
if load_ref('%ans') then   pause,end,
%ans = Sys('D')' * Sys('D');
if load_ref('%ans') then   pause,end,

//Kernel
Sys = ssrand(2, 3, 4);
[X,d,F,U,k,Z] = abinv(Sys);
%ans = ss2tf(Sys * Z);
if load_ref('%ans') then   pause,end,

%ans = ss2tf(Sys * sysfact(Sys, F, 'post') * U);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
