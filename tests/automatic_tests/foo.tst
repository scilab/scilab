getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/foo_data.ref','r');
deff('y=foo(x)', 'y=x');// define the foo function as the identity function
%ans = foo('toto');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
