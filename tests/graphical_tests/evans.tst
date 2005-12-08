getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/evans_data.ref','rb');

H = syslin('c', 352 * poly(-5, 's')/poly([0,0,2000,200,25,1], 's', 'c'));
%ans = evans(H, 100);
if load_ref('%ans') then   pause,end,

P = 3.0548543 - 8.8491842 * %i;//P=selected point
k = -1/real(horner(H, P));
Ns = H('num');Ds = H('den');
%ans = roots(Ds + k * Ns);
if load_ref('%ans') then   pause,end,
//contains P as particular root
// Another one
clf_run();s = poly(0, 's');n = 1 + s;
d = real(poly([-1,-2,-%i,%i], 's'));
evans(n, d, 100);
//
clf_run();n = real(poly([0.1 - %i,0.1 + %i,-10], 's'));
evans(n, d, 80);

xdel_run(winsid());

mclose(%U);
