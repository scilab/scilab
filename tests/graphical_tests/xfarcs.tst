getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xfarcs_data.ref','rb');
%ans = plot2d(0, 0, -1, '031', ' ', [-1,-1,1,1]);
if load_ref('%ans') then   pause,end,

arcs = [-1,0,0.5; // upper left x;
  1,0,0.5; // upper left y;
  0.5,1,0.5; // width;
  0.5,0.5,1; // height;
  0,0,0; // angle 1;
  180 * 64,360 * 64,90 * 64];// angle 2
%ans = xfarcs(arcs, [1,2,3]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
