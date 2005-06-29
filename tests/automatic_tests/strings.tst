getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/strings_data.ref','r');
%ans = ['this','is';'a 2x2','matrix'];
if load_ref('%ans') then   pause,end,

%ans = 'matrix' == ('mat' + 'rix');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
