getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sign_data.ref','rb');
%ans = sign(rand(2, 3));
if load_ref('%ans') then   pause,end,

%ans = sign(1 + %i);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
