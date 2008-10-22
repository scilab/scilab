getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/covar_data.ref','rb');
x = [10,20,30,40];
if load_ref('x') then   pause,end,

y = [10,20,30,40];
if load_ref('y') then   pause,end,

fre = [0.2,0.04,0.01,0;
  0.1,0.36,0.09,0;
  0,0.05,0.1,0;
  0,0,0,0.05];
if load_ref('fre') then   pause,end,

s = covar(x, y, fre);
if load_ref('s') then   pause,end,

xdel_run(winsid());

mclose(%U);
