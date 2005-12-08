getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/convstr_data.ref','rb');
A = ['this','is';'my','matrix'];
%ans = convstr(A, 'u');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
