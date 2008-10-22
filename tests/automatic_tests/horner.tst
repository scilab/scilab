getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/horner_data.ref','rb');
s = poly(0, 's');M = [s,1/s];
%ans = horner(M, 1);
if load_ref('%ans') then   pause,end,

%ans = horner(M, %i);
if load_ref('%ans') then   pause,end,

%ans = horner(M, 1/s);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
