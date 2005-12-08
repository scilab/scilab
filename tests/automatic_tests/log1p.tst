getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/log1p_data.ref','rb');
%ans = format('e', 24);
if load_ref('%ans') then   pause,end,

%ans = log(1.001);
if load_ref('%ans') then   pause,end,

%ans = log1p(0.001);
if load_ref('%ans') then   pause,end,

%ans = log(1 + 0.0000001);
if load_ref('%ans') then   pause,end,

%ans = log1p(0.0000001);
if load_ref('%ans') then   pause,end,

%ans = log(1 + 1.00000000000E-20);
if load_ref('%ans') then   pause,end,

%ans = log1p(1.00000000000E-20);
if load_ref('%ans') then   pause,end,

%ans = format('v');
if load_ref('%ans') then   pause,end,
//reset default format
xdel_run(winsid());

mclose(%U);
