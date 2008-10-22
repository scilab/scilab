getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/asin_data.ref','rb');
A = [1,2;3,4];
if load_ref('A') then   pause,end,

%ans = sin(asin(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
