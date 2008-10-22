getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/smooth_data.ref','rb');
x = [1,10,20,30,40];
y = [1,30,-10,20,40];
plot2d(x', y', 3, '011', ' ', [-10,-40,50,50]);
yi = smooth([x;y], 0.1);
plot2d(yi(1, :)', yi(2, :)', 1, '000');
xdel_run(winsid());

mclose(%U);
