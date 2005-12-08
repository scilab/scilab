getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/bvode_data.ref','rb');
%ans = deff('df=dfsub(x,z)', 'df=[0,0,-6/x**2,-6/x]');
if load_ref('%ans') then   pause,end,

%ans = deff('f=fsub(x,z)', 'f=(1 -6*x**2*z(4)-6*x*z(3))/x**3');
if load_ref('%ans') then   pause,end,

%ans = deff('g=gsub(i,z)', 'g=[z(1),z(3),z(1),z(3)];g=g(i)');
if load_ref('%ans') then   pause,end,

%ans = deff('dg=dgsub(i,z)', ['dg=[1,0,0,0;0,0,1,0;1,0,0,0;0,0,1,0]';
  'dg=dg(i,:)']);
if load_ref('%ans') then   pause,end,

%ans = deff('[z,mpar]=guess(x)', 'z=0;mpar=0');
if load_ref('%ans') then   pause,end,
// unused here

//define trusol for testing purposes
%ans = deff('u=trusol(x)', [
  'u=0*ones(4,1)';
  'u(1) =  0.25*(10*log(2)-3)*(1-x) + 0.5 *( 1/x   + (3+x)*log(x) - x)';
  'u(2) = -0.25*(10*log(2)-3)       + 0.5 *(-1/x^2 + (3+x)/x      + log(x) - 1)';
  'u(3) = 0.5*( 2/x^3 + 1/x   - 3/x^2)';
  'u(4) = 0.5*(-6/x^4 - 1/x/x + 6/x^3)']);
if load_ref('%ans') then   pause,end,


fixpnt = 0;m = 4;
ncomp = 1;aleft = 1;aright = 2;
zeta = [1,1,2,2];
ipar = zeros(1, 11);
ipar(3) = 1;ipar(4) = 2;ipar(5) = 2000;ipar(6) = 200;ipar(7) = 1;
ltol = [1,3];tol = [0.00000000001,0.00000000001];
res = aleft:0.1:aright;
z = bvode(res, ncomp, m, aleft, aright, zeta, ipar, ltol, tol, fixpnt, fsub, dfsub, gsub, dgsub, guess);
if load_ref('z') then   pause,end,

z1 = [];for x = res,  z1 = [z1,trusol(x)];end,
%ans = z - z1;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
