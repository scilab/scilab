getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/GlobalProperty_data.ref','rb');
// --------------------
// With the plot command :
// --------------------
x = 1:10;// Init.
%ans = plot(x, sin(x), 'colo', 'red', 'linest', '-.', 'marker', '>', 'markeredg', 'cyan', 'markerFace', 'yellow', 'markersize', 5);
if load_ref('%ans') then   pause,end,

clf_run();

// combinations' order in {PropertyName,PropertyValue} does not matter
%ans = plot(x, sin(x), 'marker', 'p', 'markerfac', 'cyan', 'markersiz', 10);
if load_ref('%ans') then   pause,end,

clf_run();

// combination of LineSpec and GlobalProperty shows the GlobalProperty predominance
%ans = plot(x, x .* x, '*cya--', 'color', 'gr', 'linestyle', '-', 'marker', 'sq', 'markersize', 6, 'markforegroun', 'red', 'markbackgro', [0.2,0.5,0.6]);
if load_ref('%ans') then   pause,end,

clf_run();

//multiple plots with different LineSpecs and finally some global GlobalProperty
clf_run();
t = 0:%pi/20:2 * %pi;
%ans = plot(t, sin(t), 'ro-.', t, cos(t), 'cya+', t, abs(sin(t)), '--mo', 'markstyl', 'diam');
if load_ref('%ans') then   pause,end,


// --------------------
// With the surf command :
// --------------------


Z = [0.0001,0.0013,0.0053,-0.0299,-0.1809,-0.2465,-0.11,-0.0168,-0.0008,-0;
  0.0005,0.0089,0.0259,-0.3673,-1.867,-2.4736,-1.0866,-0.1602,-0.0067,0;
  0.0004,0.0214,0.1739,-0.3147,-4.0919,-6.4101,-2.7589,-0.2779,0.0131,0.002;
  -0.0088,-0.0871,0.0364,1.8559,1.4995,-2.2171,-0.2729,0.8368,0.2016,0.013;
  -0.0308,-0.4313,-1.7334,-0.1148,3.0731,0.4444,2.6145,2.441,0.4877,0.0301;
  -0.0336,-0.499,-2.3552,-2.1722,0.8856,-0.0531,2.6416,2.4064,0.4771,0.0294;
  -0.0137,-0.1967,-0.8083,0.2289,3.3983,3.1955,2.4338,1.2129,0.2108,0.0125;
  -0.0014,-0.0017,0.3189,2.7414,7.1622,7.1361,3.1242,0.6633,0.0674,0.003;
  0.0002,0.0104,0.1733,1.0852,2.6741,2.6725,1.1119,0.1973,0.0152,0.0005;
  0,0.0012,0.0183,0.1099,0.2684,0.2683,0.1107,0.019,0.0014,0];

clf_run();
f = gcf();
f('figure_size') = [610,724];
%ans = subplot(211);
if load_ref('%ans') then   pause,end,

%ans = surf(Z, 'facecol', 'interp', 'ydat', 101:110, 'edgecol', 'mage');
if load_ref('%ans') then   pause,end,

%ans = subplot(212);
if load_ref('%ans') then   pause,end,

%ans = surf(Z, 'edgeco', 'b', 'marker', 'd', 'markersiz', 9, 'markerfac', 'k', 'xdata', -50:-41);
if load_ref('%ans') then   pause,end,



xdel_run(winsid());

mclose(%U);
