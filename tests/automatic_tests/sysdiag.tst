getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sysdiag_data.ref','rb');
s = poly(0, 's');
if load_ref('s') then   pause,end,

%ans = sysdiag(rand(2, 2), 1/(s + 1), [1/(s - 1);1/((s - 2) * (s - 3))]);
if load_ref('%ans') then   pause,end,

%ans = sysdiag(tf2ss(1/s), 1/(s + 1), [1/(s - 1);1/((s - 2) * (s - 3))]);
if load_ref('%ans') then   pause,end,


s = poly(0, 's');
if load_ref('s') then   pause,end,

%ans = sysdiag(rand(2, 2), 1/(s + 1), [1/(s - 1);1/((s - 2) * (s - 3))]);
if load_ref('%ans') then   pause,end,

%ans = sysdiag(tf2ss(1/s), 1/(s + 1), [1/(s - 1);1/((s - 2) * (s - 3))]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
