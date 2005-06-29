getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/tan_data.ref','r');
x = [1,%i,-1,-%i];
if load_ref('x') then   pause,end,

%ans = tan(x);
if load_ref('%ans') then   pause,end,

%ans = sin(x) ./ cos(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
