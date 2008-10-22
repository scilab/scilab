getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/csim_data.ref','rb');
s = poly(0, 's');rand('seed', 0);w = ssrand(1, 1, 3);w('A') = w('A') - 2 * eye();
t = 0:0.05:5;
//impulse(w) = step (s * w)
xbasc_run(0);xset('window', 0);xselect();
%ans = plot2d([t',t'], [csim('step', t, tf2ss(s) * w)',0 * t']);
if load_ref('%ans') then   pause,end,

xbasc_run(1);xset('window', 1);xselect();
%ans = plot2d([t',t'], [csim('impulse', t, w)',0 * t']);
if load_ref('%ans') then   pause,end,

//step(w) = impulse (s^-1 * w)
xbasc_run(3);xset('window', 3);xselect();
%ans = plot2d([t',t'], [csim('step', t, w)',0 * t']);
if load_ref('%ans') then   pause,end,

xbasc_run(4);xset('window', 4);xselect();
%ans = plot2d([t',t'], [csim('impulse', t, tf2ss(1/s) * w)',0 * t']);
if load_ref('%ans') then   pause,end,


//input defined by a time function
%ans = deff('u=input(t)', 'u=abs(sin(t))');
if load_ref('%ans') then   pause,end,

xbasc_run();%ans = plot2d([t',t'], [csim(input, t, w)',0 * t']);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
