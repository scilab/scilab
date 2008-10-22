getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/modulo_data.ref','rb');
n = [1,2,10,15];m = [2,2,3,5];
%ans = modulo(n, m);
if load_ref('%ans') then   pause,end,


%ans = modulo(-3, 9);
if load_ref('%ans') then   pause,end,

%ans = pmodulo(-3, 9);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
