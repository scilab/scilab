getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/rgb2name_data.ref','rb');
%ans = rgb2name(255, 128, 128);
if load_ref('%ans') then   pause,end,

%ans = rgb2name([255,215,0]);
if load_ref('%ans') then   pause,end,

// get color #10 of the current colormap and find its name
cmap = get(gcf(), 'color_map');
%ans = rgb2name(cmap(10, :) * 255);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
