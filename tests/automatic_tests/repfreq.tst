getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/repfreq_data.ref','rb');
A = diag([-1,-2]);B = [1;1];C = [1,1];
Sys = syslin('c', A, B, C);
frq = 0:0.02:5;w = frq * 2 * %pi;//frq=frequencies in Hz ;w=frequencies in rad/sec;
[frq1,rep] = repfreq(Sys, frq);
[db,phi] = dbphi(rep);
Systf = ss2tf(Sys);
if load_ref('Systf') then   pause,end,
//Transfer function of Sys
x = horner(Systf, w(2) * sqrt(-1));
if load_ref('x') then   pause,end,
// x is Systf(s) evaluated at s = i w(2)
rep = 20 * log(abs(x))/log(10);
if load_ref('rep') then   pause,end,
//magnitude of x in dB
%ans = db(2);
if load_ref('%ans') then   pause,end,
// same as rep
ang = atan(imag(x), real(x));//in rad.
ang = ang * 180/%pi;
if load_ref('ang') then   pause,end,
//in degrees
%ans = phi(2);
if load_ref('%ans') then   pause,end,

repf = repfreq(Sys, frq);
%ans = repf(2) - x;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
