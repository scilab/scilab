getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/browsevar_data.ref','rb');
browsevar();
xdel_run(winsid());

mclose(%U);
