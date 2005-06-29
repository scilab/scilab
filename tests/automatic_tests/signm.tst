getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/signm_data.ref','r');
A = rand(4, 4);B = A + A';X = signm(B);%ans = spec(B);
if load_ref('%ans') then   pause,end,
%ans = spec(X);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
