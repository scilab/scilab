getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/stabil_data.ref','rb');
// Gain:
Sys = ssrand(0, 2, 5, list('st', 2, 3, 3));
A = Sys('A');B = Sys('B');F = stabil(A, B);
%ans = spec(A);
if load_ref('%ans') then   pause,end,
//2 controllable modes 2 unstable uncontrollable modes
//and one stable uncontrollable mode
%ans = spec(A + B * F);
if load_ref('%ans') then   pause,end,
//the two controllable modes are set to -1.
// Compensator:
Sys = ssrand(3, 2, 5, list('st', 2, 3, 3));//3 outputs, 2 inputs, 5 states
//2 controllables modes, 3 controllable or stabilizable modes.
K = stabil(Sys, -2, -3);//Compensator for Sys.
%ans = spec(Sys('A'));
if load_ref('%ans') then   pause,end,

%ans = spec(h_cl(Sys, K));
if load_ref('%ans') then   pause,end,
//K Stabilizes what can be stabilized.
xdel_run(winsid());

mclose(%U);
