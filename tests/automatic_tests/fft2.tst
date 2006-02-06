getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fft2_data.ref','rb');
// ....


xdel_run(winsid());

mclose(%U);
