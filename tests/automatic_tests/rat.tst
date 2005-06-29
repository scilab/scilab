getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/rat_data.ref','r');
[n,d] = rat(%pi);
if load_ref('d') then   pause,end,
if load_ref('n') then   pause,end,

[n,d] = rat(%pi, 0.000000000001);
if load_ref('d') then   pause,end,
if load_ref('n') then   pause,end,

%ans = n/d - %pi;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
