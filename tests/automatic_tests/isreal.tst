getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/isreal_data.ref','rb');
%ans = isreal([1,2]);
if load_ref('%ans') then   pause,end,

%ans = isreal(1 + 0 * %i);
if load_ref('%ans') then   pause,end,

%ans = isreal(1 + 0 * %i, 0);
if load_ref('%ans') then   pause,end,

%ans = isreal(1 + %s);
if load_ref('%ans') then   pause,end,

%ans = isreal(sprand(3, 3, 0.1));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
