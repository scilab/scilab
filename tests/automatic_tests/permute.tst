getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/permute_data.ref','rb');
//example 1:
x = [1,2,3;4,5,6];
y = permute(x, [2,1]);

//example 2:
x = matrix(1:12, [2,3,2]);
y = permute(x, [3,1,2]);
xdel_run(winsid());

mclose(%U);
