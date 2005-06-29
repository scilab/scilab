getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/eval_data.ref','r');
a = 1;b = 2;Z = ['a','sin(b)'];%ans = eval(Z);
if load_ref('%ans') then   pause,end,
//returns the matrix [1,0.909];
xdel_run(winsid());

mclose(%U);
