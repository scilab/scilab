getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/norm_data.ref','r');
A = [1,2,3];
%ans = norm(A, 1);
if load_ref('%ans') then   pause,end,

%ans = norm(A, 'inf');
if load_ref('%ans') then   pause,end,

A = [1,2;3,4];
if load_ref('A') then   pause,end,

%ans = max(svd(A)) - norm(A);
if load_ref('%ans') then   pause,end,


A = sparse([1,0,0,33,-1]);
if load_ref('A') then   pause,end,

%ans = norm(A);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
