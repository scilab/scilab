getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/glue_data.ref','r');


xdel_run(winsid());

mclose(%U);
