getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ndims_data.ref','rb');
A = rand(2, 3);
%ans = ndims(A);
if load_ref('%ans') then   pause,end,


A = rand(2, 3, 2);
%ans = size(A);
if load_ref('%ans') then   pause,end,
%ans = ndims(A);
if load_ref('%ans') then   pause,end,


H = [1/%s,1/(%s + 1)];
if load_ref('H') then   pause,end,

%ans = ndims(H);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
