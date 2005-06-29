getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/sign_data.ref','r');
%ans = sign(rand(2, 3));
if load_ref('%ans') then   pause,end,

%ans = sign(1 + %i);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
