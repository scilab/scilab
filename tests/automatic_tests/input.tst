getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/input_data.ref','rb');
//x=input("How many iterations?")
//x=input("What is your name?","string")
xdel_run(winsid());

mclose(%U);
