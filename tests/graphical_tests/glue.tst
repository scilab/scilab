getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/glue_data.ref','r');


xdel_run(winsid());

mclose(%U);
