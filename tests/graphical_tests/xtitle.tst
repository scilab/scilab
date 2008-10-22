getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xtitle_data.ref','rb');
// draw a surface
plot3d();
// puts the titles
xtitle('My surface is blue', 'X axis', 'Y axis', 'Z axis');
// draw a box around the titles
xtitle('My surface is blue', 'X axis', 'Y axis', 'Z axis', boxed=1);
xdel_run(winsid());

mclose(%U);
