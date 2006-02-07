getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/Scilab_data.ref','rb');
// See SCI/examples/Callsci/CallsciJava directory
xdel_run(winsid());

mclose(%U);
