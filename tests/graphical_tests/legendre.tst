getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/legendre_data.ref','rb');
// example 1 : plot of the 6 first Legendre polynomials on (-1,1)
l = nearfloat('pred', 1);
x = linspace(-l, l, 200)';
y = legendre(0:5, 0, x);
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y', leg='p0@p1@p2@p3@p4@p5@p6');
if load_ref('%ans') then   pause,end,

%ans = xtitle('the 6 th first Legendre polynomials');
if load_ref('%ans') then   pause,end,


// example 2 : plot of the associated Legendre functions of degree 5
l = nearfloat('pred', 1);
x = linspace(-l, l, 200)';
y = legendre(5, 0:5, x, 'norm');
%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = plot2d(x, y', leg='p5,0@p5,1@p5,2@p5,3@p5,4@p5,5');
if load_ref('%ans') then   pause,end,

%ans = xtitle('the (normalised) associated Legendre functions of degree 5');
if load_ref('%ans') then   pause,end,


// example 3 : define then plot a spherical harmonic
// 3-1 : define the function Ylm
function [y] = Y(l,m,theta,phi)
     // theta may be a scalar or a row vector
     // phi may be a scalar or a column vector
     if m >= 0 then
        y = (-1)^m/(sqrt(2*%pi))*exp(%i*m*phi)*legendre(l, m, cos(theta), "norm")
     else
        y = 1/(sqrt(2*%pi))*exp(%i*m*phi)*legendre(l, -m, cos(theta), "norm")
     end
endfunction
if load_ref('%ans') then   pause,end,


// 3.2 : define another useful function
function [x,y,z] = sph2cart(theta,phi,r)
     // theta row vector      1 x nt
     // phi   column vector  np x 1
     // r     scalar or np x nt matrix (r(i,j) the length at phi(i) theta(j))
     x = r.*(cos(phi)*sin(theta));
     y = r.*(sin(phi)*sin(theta));
     z = r.*(ones(phi)*cos(theta));
endfunction
if load_ref('%ans') then   pause,end,


// 3-3 plot Y31(theta,phi)
l = 3;m = 1;
theta = linspace(0.1, %pi - 0.1, 60);
phi = linspace(0, 2 * %pi, 120)';
f = Y(l, m, theta, phi);
[x1,y1,z1] = sph2cart(theta, phi, abs(f));[xf1,yf1,zf1] = nf3d(x1, y1, z1);
[x2,y2,z2] = sph2cart(theta, phi, abs(real(f)));[xf2,yf2,zf2] = nf3d(x2, y2, z2);
[x3,y3,z3] = sph2cart(theta, phi, abs(imag(f)));[xf3,yf3,zf3] = nf3d(x3, y3, z3);

%ans = xbasc_run();
if load_ref('%ans') then   pause,end,

%ans = subplot(1, 3, 1);
if load_ref('%ans') then   pause,end,

plot3d(xf1, yf1, zf1, flag=[2,4,4]);%ans = xtitle('|Y31(theta,phi)|');
if load_ref('%ans') then   pause,end,

%ans = subplot(1, 3, 2);
if load_ref('%ans') then   pause,end,

plot3d(xf2, yf2, zf2, flag=[2,4,4]);%ans = xtitle('|Real(Y31(theta,phi))|');
if load_ref('%ans') then   pause,end,

%ans = subplot(1, 3, 3);
if load_ref('%ans') then   pause,end,

plot3d(xf3, yf3, zf3, flag=[2,4,4]);%ans = xtitle('|Imag(Y31(theta,phi))|');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
