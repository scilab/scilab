getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fprintfMat_data.ref','rb');
n = 50;
a = rand(n, n, 'u');
fprintfMat(TMPDIR + '/Mat', a, '%5.2f');
a1 = fscanfMat(TMPDIR + '/Mat');
xdel_run(winsid());

mclose(%U);
