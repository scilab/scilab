getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/coth_data.ref','r');
x = [1,2 * %i];
if load_ref('x') then   pause,end,

t = exp(x);
%ans = (t - ones(x) ./ t) .\ (t + ones(x) ./ t);
if load_ref('%ans') then   pause,end,

%ans = coth(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
