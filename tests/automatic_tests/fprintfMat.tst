getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/fprintfMat_data.ref','r');
n = 50;
a = rand(n, n, 'u');
fprintfMat(TMPDIR + '/Mat', a, '%5.2f');
a1 = fscanfMat(TMPDIR + '/Mat');
xdel_run(winsid());

mclose(%U);
