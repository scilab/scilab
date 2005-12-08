getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/interpln_data.ref','rb');
x = [1,10,20,30,40];
y = [1,30,-10,20,40];
plot2d(x', y', -3, '011', ' ', [-10,-40,50,50]);
yi = interpln([x;y], -4:45);
plot2d((-4:45)', yi', 3, '000');
xdel_run(winsid());

mclose(%U);
