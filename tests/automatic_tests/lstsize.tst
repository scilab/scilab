getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/lstsize_data.ref','r');
%ans = lstsize(list(1, 'aqsdf'));
if load_ref('%ans') then   pause,end,

x = ssrand(3, 2, 4);
[ny,nu] = size(x);
if load_ref('nu') then   pause,end,
if load_ref('ny') then   pause,end,

%ans = lstsize(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
