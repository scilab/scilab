getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/surf_data.ref','rb');
// Z initialisation

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

//simple surface
surf(Z);// Note that X and Y are determined by Z dimensions

//same surface with red face color and blue edges
scf(2);// new figure number 2
%ans = surf(Z, 'facecol', 'red', 'edgecol', 'blu');
if load_ref('%ans') then   pause,end,


// X and Y initialisation
// NB: here, X has the same lines and Y the same columns
X = [-3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3;
  -3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3];

Y = [-3,-3,-3,-3,-3,-3,-3,-3,-3,-3;
  -2.3333,-2.3333,-2.3333,-2.3333,-2.3333,-2.3333,-2.3333,-2.3333,-2.3333,-2.3333;
  -1.6667,-1.6667,-1.6667,-1.6667,-1.6667,-1.6667,-1.6667,-1.6667,-1.6667,-1.6667;
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1;
  -0.3333,-0.3333,-0.3333,-0.3333,-0.3333,-0.3333,-0.3333,-0.3333,-0.3333,-0.3333;
  0.3333,0.3333,0.3333,0.3333,0.3333,0.3333,0.3333,0.3333,0.3333,0.3333;
  1,1,1,1,1,1,1,1,1,1;
  1.6667,1.6667,1.6667,1.6667,1.6667,1.6667,1.6667,1.6667,1.6667,1.6667;
  2.3333,2.3333,2.3333,2.3333,2.3333,2.3333,2.3333,2.3333,2.3333,2.3333;
  3,3,3,3,3,3,3,3,3,3];

// example 1
%ans = scf(3);
if load_ref('%ans') then   pause,end,

%ans = surf(X, Y, Z);
if load_ref('%ans') then   pause,end,


//example 2
// As you can see, the grid is not necessary rectangular
%ans = scf(4);
if load_ref('%ans') then   pause,end,

X(1, 4) = -1.5;
Y(1, 4) = -3.5;
Z(1, 4) = -2;
%ans = surf(X, Y, Z);
if load_ref('%ans') then   pause,end,


// example 3
// X and Y are vectors =&gt; same behavior as sample 1
// With vectors, the grid is inevitably rectangular
%ans = scf(5);
if load_ref('%ans') then   pause,end,
// new figure number 5
X = [-3,-2.3333,-1.6667,-1,-0.3333,0.3333,1,1.6667,2.3333,3];
Y = X;
%ans = surf(X, Y, Z);
if load_ref('%ans') then   pause,end,



//LineSpec and GlobalProperty examples:
%ans = xdel_run(winsid());
if load_ref('%ans') then   pause,end,
// destroy all existing figures
%ans = surf(Z, Z + 5);
if load_ref('%ans') then   pause,end,
// color array specified
e = gce();
e('cdata_mapping') = 'direct';
if load_ref('e') then   pause,end,
// default is 'scaled' relative to the colormap
e('color_flag') = 3;// interpolated shading mode. The default is 4 ('flat' mode) for surf

%ans = scf(2);
if load_ref('%ans') then   pause,end,

%ans = surf(X, Y, Z, 'colorda', ones(10, 10), 'edgeco', 'cya', 'marker', 'penta', 'markersiz', 20, 'markeredg', 'yel', 'ydata', 56:65);
if load_ref('%ans') then   pause,end,


%ans = scf(3);
if load_ref('%ans') then   pause,end,

%ans = surf(Z, 'cdatamapping', 'direct');
if load_ref('%ans') then   pause,end,

%ans = scf(4);
if load_ref('%ans') then   pause,end,

%ans = surf(Z, 'facecol', 'interp');
if load_ref('%ans') then   pause,end,
// interpolated shading mode (color_flag == 3)

%ans = scf(10);
if load_ref('%ans') then   pause,end,

axfig10 = gca();
scf(11);
%ans = surf(axfig10, Z, 'ydat', 100:109, 'marker', 'd', 'markerfac', 'green', 'markeredg', 'yel');
if load_ref('%ans') then   pause,end,
// draw onto the axe of figure 10

%ans = xdel_run(winsid());
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
