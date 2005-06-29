getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/prod_data.ref','r');
A = [1,2;0,100];
%ans = prod(A);
if load_ref('%ans') then   pause,end,

%ans = prod(A, 'c');
if load_ref('%ans') then   pause,end,

%ans = prod(A, 'r');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
