getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/plot3d3_data.ref','rb');

u = linspace(-%pi/2, %pi/2, 40);
v = linspace(0, 2 * %pi, 20);
X = cos(u)' * cos(v);
Y = cos(u)' * sin(v);
Z = sin(u)' * ones(v);
plot3d3(X, Y, Z);
// New Graphic mode only
e = gce();// get the current entity
e('visible') = 'off';
e('visible') = 'on';// back to the mesh view


xdel_run(winsid());

mclose(%U);
