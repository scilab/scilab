getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sci2exp_data.ref','rb');
a = [1,2;3,4];
if load_ref('a') then   pause,end,

%ans = sci2exp(a, 'aa');
if load_ref('%ans') then   pause,end,

%ans = sci2exp(a, 'aa', 0);
if load_ref('%ans') then   pause,end,

%ans = sci2exp(ssrand(2, 2, 2));
if load_ref('%ans') then   pause,end,

%ans = sci2exp(poly([1,0,3,4], 's'), 'fi');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
