getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/convstr_data.ref','r');
A = ['this','is';'my','matrix'];
%ans = convstr(A, 'u');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
