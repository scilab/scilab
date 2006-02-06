getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/mesh_data.ref','rb');
[X,Y] = meshgrid(-1:0.1:1, -1:0.1:1);
Z = X.^2 - Y.^2;
xtitle('z=x2-y ^2');
mesh(X, Y, Z);
xdel_run(winsid());

mclose(%U);
