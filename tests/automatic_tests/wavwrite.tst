getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/wavwrite_data.ref','rb');
A = matrix(1:6, 2, 3);
wavwrite(A/6, TMPDIR + '/foo.wav');
B = wavread(TMPDIR + '/foo.wav');
%ans = maxi(abs(A - round(B * 6)));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
