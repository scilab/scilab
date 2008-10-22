getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/macrovar_data.ref','rb');
%ans = deff('y=f(x1,x2)', 'loc=1;y=a*x1+x2-loc');
if load_ref('%ans') then   pause,end,

vars = macrovar(f);
if load_ref('vars') then   pause,end,

xdel_run(winsid());

mclose(%U);
