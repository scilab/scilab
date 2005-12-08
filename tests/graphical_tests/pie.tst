getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/pie_data.ref','rb');

// First example : one input argument  x=[1 2 5]
scf(0);
pie([1,2,5]);

// Second example : two input arguments x=[5 9  4 6 3], sp=[0 1 0 1 0], the second and the fourth are separated of the others
scf(1);
pie([5,9,4,6,3], [0,1,0,1,0]);

// Third example : three input arguments, x=[3 4 6 2], sp=[0 1 0 0], txt=["part1","part2","part3","part4"]
scf(2);
pie([3,4,6,2], [0,1,0,0], ['part1','part2','part3','part4']);
xdel_run(winsid());

mclose(%U);
