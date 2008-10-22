getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/freq_data.ref','rb');
s = poly(0, 's');
sys = (s + 1)/(s^3 - 5 * s + 4);
if load_ref('sys') then   pause,end,

rep = freq(sys('num'), sys('den'), [0,0.9,1.1,2,3,10,20]);
if load_ref('rep') then   pause,end,

%ans = [horner(sys, 0),horner(sys, 20)];
if load_ref('%ans') then   pause,end,

//
Sys = tf2ss(sys);
[A,B,C,D] = abcd(Sys);
%ans = freq(A, B, C, [0,0.9,1.1,2,3,10,20]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
