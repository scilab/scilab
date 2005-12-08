getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/oldload_data.ref','rb');
a = eye(2, 2);b = ones(a);
oldsave(TMPDIR + '/vals.dat', a, b);
%ans = clear('a');
if load_ref('%ans') then   pause,end,

%ans = clear('b');
if load_ref('%ans') then   pause,end,

oldload(TMPDIR + '/vals.dat', 'a', 'b');
xdel_run(winsid());

mclose(%U);
