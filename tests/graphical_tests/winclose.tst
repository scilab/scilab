getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/winclose_data.ref','rb');
//CREATE SOME WINDOWS
win1 = waitbar('This is an example');
win2 = waitbar('HELLO!');
halt();
winclose([win1,win2]);
xdel_run(winsid());

mclose(%U);
