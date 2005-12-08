getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/frep2tf_data.ref','rb');
s = poly(0, 's');
h = syslin('c', (s - 1)/(s^3 + 5 * s + 20));
if load_ref('h') then   pause,end,

frq = 0:0.05:3;repf = repfreq(h, frq);
%ans = clean(frep2tf(frq, repf, 3));
if load_ref('%ans') then   pause,end,


Sys = ssrand(1, 1, 10);
frq = logspace(-3, 2, 200);
[frq,rep] = repfreq(Sys, frq);//Frequency response of Sys
[Sys2,err] = frep2tf(frq, rep, 10);Sys2 = clean(Sys2);
if load_ref('Sys2') then   pause,end,
//Sys2 obtained from freq. resp of Sys
[frq,rep2] = repfreq(Sys2, frq);//Frequency response of Sys2
xbasc_run();%ans = bode(frq, [rep;rep2]);
if load_ref('%ans') then   pause,end,
//Responses of Sys and Sys2
%ans = [sort(spec(Sys('A'))),sort(roots(Sys2('den')))];
if load_ref('%ans') then   pause,end,
//poles

dom = 1/1000;// Sampling time
z = poly(0, 'z');
h = syslin(dom, (z^2 + 0.5)/(z^3 + 0.1 * (z^2) - 0.5 * z + 0.08));
if load_ref('h') then   pause,end,

frq = (0:0.01:0.5)/dom;repf = repfreq(h, frq);
[Sys2,err] = frep2tf(frq, repf, 3, dom);
[frq,rep2] = repfreq(Sys2, frq);//Frequency response of Sys2
xbasc_run();plot2d1('onn', frq', abs([repf;rep2])');

xdel_run(winsid());

mclose(%U);
