getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/eval_data.ref','rb');
a = 1;b = 2;Z = ['a','sin(b)'];%ans = eval(Z);
if load_ref('%ans') then   pause,end,
//returns the matrix [1,0.909];
xdel_run(winsid());

mclose(%U);
