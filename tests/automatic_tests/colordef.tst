getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/colordef_data.ref','r');
// Add here scilab instructions and comments
xdel_run(winsid());

mclose(%U);
