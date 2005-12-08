getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/mrfit_data.ref','rb');
w = 0.01:0.01:2;s = poly(0, 's');
G = syslin('c', 2 * (s^2 + 0.1 * s + 2), (s^2 + s + 1) * (s^2 + 0.3 * s + 1));// syslin('c',Num,Den);
fresp = repfreq(G, w);
mag = abs(fresp);
Gid = mrfit(w, mag, 4);
frespfit = repfreq(Gid, w);
%ans = plot2d([w',w'], [mag(:),abs(frespfit(:))]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
