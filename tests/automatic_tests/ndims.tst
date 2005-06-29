getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/ndims_data.ref','r');
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
