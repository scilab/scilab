getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/coth_data.ref','rb');
x = [1,2 * %i];
if load_ref('x') then   pause,end,

t = exp(x);
%ans = (t - ones(x) ./ t) .\ (t + ones(x) ./ t);
if load_ref('%ans') then   pause,end,

%ans = coth(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
