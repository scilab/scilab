function [xdot]=car(t,x)
//
//
// Copyright INRIA
xdot=zeros(1,4) ;
// car lenght for the control computation
LCpct = bigL * 1.;
// calcul de u1 et u2
//
if (t<0)
  u1=0 ; u2=0 ; 
elseif (t < bigT) 
//      motion 1
 tau = t/bigT ;
 phi=tau*tau*(3-2*tau) ;
 dphi = 6*tau*(1-tau) ; 
 a = (1-phi)*a1 + phi*a0 ;
 da = ((a0-a1)/bigT) * dphi ; 
 f   =   p(1).*(a-a0)^3 +    p(2).*(a-a0)^4 +    p(3).*(a-a0)^5 ;
 df  = 3*p(1).*(a-a0)^2 +  4*p(2).*(a-a0)^3 +  5*p(3).*(a-a0)^4 ;
 ddf = 6*p(1).*(a-a0)   + 12*p(2).*(a-a0)^2 + 20*p(3).*(a-a0)^3 ;
 dddf= 6*p(1).*1        + 24*p(2).*(a-a0)   + 60*p(3).*(a-a0)^2 ;
 k = ddf / ((1+df*df)^(3/2)) ;
 dk = ( dddf - 3*df*ddf*ddf/(1+df*df)) / ((1+df*df)^(3/2)) ;
 u1 = ((1+df*df)^(1/2)) * da ;
 u2 =  (bigL*dk / (1+(bigL*k)^2) ) * da ;
elseif (t < (2*bigT) ) 
//     motion 2 
 tau = t/bigT -1 ;
 phi=tau*tau*(3-2*tau) ;
 dphi = 6*tau*(1-tau) ; 
 a = (1-phi)*a0 + phi*a1 ;
 da = ((a1-a0)/bigT) * dphi ; 
 f   =   p(1).*(a-a0)^3 +    p(2).*(a-a0)^4 +    p(3).*(a-a0)^5 ;
 df  = 3*p(1).*(a-a0)^2 +  4*p(2).*(a-a0)^3 +  5*p(3).*(a-a0)^4 ;
 ddf = 6*p(1).*(a-a0)   + 12*p(2).*(a-a0)^2 + 20*p(3).*(a-a0)^3 ;
 dddf= 6*p(1).*1        + 24*p(2).*(a-a0)   + 60*p(3).*(a-a0)^2 ;
 k = ddf / ((1+df*df)^(3/2)) ;
 dk = ( dddf - 3*df*ddf*ddf/(1+df*df)) / ((1+df*df)^(3/2)) ;
 u1 = ((1+df*df)^(1/2)) * da ;
 u2 =  (LC*dk / (1+(LC*k)^2) ) * da ;
else
 u1=0; u2=0;
end
xdot(1)= u1 * cos(x(1,3)) ;
xdot(2)= u1 * sin(x(1,3)) ;
xdot(3)= u1 * tan(x(1,4)) / bigL ;
xdot(4)= u2 ;

function [xdot]=car2T(t,x)
//
xdot=zeros(1,6) ;
// calcul de u1 et u2
//
if (t<0)
  u1=0 ; u2=0 ; 
elseif (t < bigT) 
//      motion 
 tau = t/bigT ;
 phi=tau*tau*(3-2*tau) ;
 dphi = 6*tau*(1-tau) ; 
 a = (1-phi)*a1 + phi*a0 ;
 da = ((a0-a1)/bigT) * dphi ; 
 [ff df d2f d3f d4f d5f] = cr2Tfjt(a) ;
 [k2 k1 k0 dk0]=cr2Tfk(df,d2f,d3f,d4f,d5f) ;
 u1 =  ( (1+(d2*k2)^2)*(1+(d1*k1)^2)*(1+df^2) )^(1/2) * da ;
 u2 =  da * bigL * dk0 / (1+(bigL*k0)^2) ;
else
 u1=0; u2=0;
end
xdot(1)= u1 * cos(x(1,3)) ;
xdot(2)= u1 * sin(x(1,3)) ;
xdot(3)= u1 * tan(x(1,6)) / bigL ;
xdot(4)= u1 * sin(x(1,3)-x(1,4)) / d1 ; 
xdot(5)= u1 * sin(x(1,4)-x(1,5)) * cos(x(1,3)-x(1,4)) / d2 ;
xdot(6)= u2 ;

function [ff,df,d2f,d3f,d4f,d5f]=cr2Tfjt(a)
//
//
M= [
    (a-a0)^5      (a-a0)^6     (a-a0)^7     (a-a0)^8       (a-a0)^9 
  5*(a-a0)^4    6*(a-a0)^5   7*(a-a0)^6    8*(a-a0)^7    9*(a-a0)^8 
 20*(a-a0)^3   30*(a-a0)^4  42*(a-a0)^5   56*(a-a0)^6   72*(a-a0)^7 
 60*(a-a0)^2  120*(a-a0)^3 210*(a-a0)^4  336*(a-a0)^5  504*(a-a0)^6 
120*(a-a0)^1  360*(a-a0)^2 840*(a-a0)^3 1680*(a-a0)^4 3024*(a-a0)^5 
120  720*(a-a0)^1 2520*(a-a0)^2 6720*(a-a0)^3 15120*(a-a0)^4 
      ]*p ;
ff  = M(1) + b0 ;
df  = M(2) ;
d2f = M(3) ;
d3f = M(4) ;
d4f = M(5) ;
d5f = M(6) ;


function [k2,k1,k0,dk0]=cr2Tfk(df,d2f,d3f,d4f,d5f)
//
//
// computation of curvatures from derivatives of b=f(a)
//
g = (1+df^2)^(-1/2);
dg = - df*d2f*g^3 ;
d2g = - g*g*(d2f^2*g+df*d3f*g+3*df*d2f*dg) ;
d3g = ....
- 2*g*dg*(d2f^2*g+df*d3f*g+3*df*d2f*dg) ....
- g^2*(3*d2f*d3f*g+df*d4f*g ....
       + 4*d2f^2*dg+4*df*d3f*dg+3*df*d2f*d2g) ;
//
k2  = d2f * g^3  ;
dk2 = d3f*g^3 + 3*d2f*g^2*dg ;
d2k2 = g^2*(d4f*g+6*d3f*dg+3*d2f*d2g) + 6*g*dg^2*d2f ;
d3k2 = 2*g*dg*(d4f*g+6*d3f*dg+3*d2f*d2g) ....
 + g^2*(d5f*g+7*d4f*dg+9*d3f*d2g+3*d2f*d3g) ....
 +6*dg^3*d2f+12*g*dg*d2g*d2f+6*g*dg^2*d3f ;
//
g2 = (1+(d2*k2)^2)^(-1/2) ;
dg2 = -d2^2*k2*dk2*g2^3 ;
d2g2 = -d2^2*g2^2*(dk2^2*g2+k2*d2k2*g2+3*k2*dk2*dg2) ;
//
h2 = g2^3*g ;
dh2 = g2^2*(3*dg2*g+g2*dg);
d2h2 = 2*g2*dg2*(3*dg2*g+g2*dg) ....
     + g2^2*(3*d2g2*g+4*dg2*dg+g2*d2g) ;
//
k1 = g2*k2 + d2*h2*dk2 ;
dk1 = dg2*k2 + g2*dk2 + d2 * (dh2*dk2+h2*d2k2) ;
d2k1 = d2g2*k2 + 2*dg2*dk2 + g2*d2k2 ....
   + d2 * (d2h2*dk2+2*dh2*d2k2+h2*d3k2) ;
//
g1 = (1+(d1*k1)^2)^(-1/2) ;
dg1 = - d1^2*k1*dk1*g1^3 ;
//
k0 = g1*k1 + d1*g1^3*g2*g*dk1 ;
dk0 = dg1*k1 + g1*dk1 ....
 + d1*g1^2*(3*dg1*g2*g*dk1+g1*dg2*g*dk1 ....
          +  g1*g2*dg*dk1+g1*g2*g*d2k1) ;



function coef=cr2Tkf(b,theta2,theta12,theta01,phi)
//
//
M = [ 
  1*(a1-a0)^5    1*(a1-a0)^6   1*(a1-a0)^7    1*(a1-a0)^8    1*(a1-a0)^9 
  5*(a1-a0)^4    6*(a1-a0)^5   7*(a1-a0)^6    8*(a1-a0)^7    9*(a1-a0)^8 
 20*(a1-a0)^3   30*(a1-a0)^4  42*(a1-a0)^5   56*(a1-a0)^6   72*(a1-a0)^7 
 60*(a1-a0)^2  120*(a1-a0)^3 210*(a1-a0)^4  336*(a1-a0)^5  504*(a1-a0)^6 
120*(a1-a0)^1  360*(a1-a0)^2 840*(a1-a0)^3 1680*(a1-a0)^4 3024*(a1-a0)^5 
      ] ;
//
//
df = tan(theta2) ;
//
// curvatures
k2=tan(theta12)/d2;k1=tan(theta01)/d1;k0=tan(phi)/bigL;
// 
ddf = k2*((1+df*df)^(3/2)) ;
//
// derivative of k2
dk2ds2 = ( (1+(d2*k2)^2)/d2 )*( (1+(d2*k2)^2)^(1/2)*k1 - k2 ) ;
dk2 = (1+df*df)^(1/2) * dk2ds2 ;
//
dddf = dk2 * (1+df*df)^(3/2)  + 3*k2*df*ddf*(1+df*df)^(1/2) ;
//
// second derivative of k2
dk1ds1 = ( (1+(d1*k1)^2)/d1 )*( (1+(d1*k1)^2)^(1/2)*k0 - k1 ) ;
dk1ds2 = dk1ds1 *  (1+(d2*k2)^2)^(1/2) ;
//
ddk2ds2 =  ....
  3*d2*k2*dk2ds2*(1+(d2*k2)^2)^(1/2)*k1 ....
+ (1+(d2*k2)^2)^(3/2)*dk1ds2/d2 ...
- (1/d2+3*d2*k2*k2)*dk2ds2 ;
//
ddk2 = .... 
   df*ddf*((1+df*df)^(-1/2))*dk2ds2 ....
 + (1+df*df)*ddk2ds2 ; 
//
ddddf =  ....
   ddk2 * (1+df*df)^(3/2) ....
 + 6*dk2*df*ddf*(1+df*df)^(1/2) ....
 + 3*k2*ddf*ddf*(1+df*df)^(1/2) ....
 + 3*k2*df*dddf*(1+df*df)^(1/2) ....
 + 3*k2*(df*ddf)^2*(1+df*df)^(-1/2) ;
//
//
//
//
coef =inv(M)* [b;df;ddf;dddf;ddddf] ;

function []=ptcr(state)
//
x=state(1);y=state(2);theta=state(3);phi=state(4);
// 
// plot the car 
//
// rear wheels
wheel1 =  [ -1/8 1/8 ;  1/6  1/6 ] ; 
wheel2 =  [ -1/8 1/8 ; -1/6 -1/6 ] ;
// front wheels
wheel3 = [(1-cos(phi)/8)   (1+cos(phi)/8);
(1/6-sin(phi)/8) (1/6+sin(phi)/8)   ] ;
wheel4 = [(1-cos(phi)/8)    (1+cos(phi)/8);
(-1/6-sin(phi)/8) (-1/6+sin(phi)/8)  ] ;
// 
// car + wheels
xy_car = [ [  -1/3   7/6   4/3  4/3  7/6  -1/3  -1/3;
              -1/3  -1/3  -1/6  1/6  1/3   1/3  -1/3;
         ] wheel1 wheel2 wheel3 wheel4 ] ;
xy_car = ....
  [x x x x x x x x x x x x x x x;
   y y y y y y y y y y y y y y y ] + .... 
   bigL*[cos(theta) -sin(theta);sin(theta) cos(theta)]*xy_car  ;
//  
xpoly(xy_car(1,1:7),xy_car(2,1:7),'lines') ;
// The 4 wheels
xpolys(matrix(xy_car(1,8:15),2,4),matrix(xy_car(2,8:15),2,4),[1 1 1 1])


function ptcr2T(state)
// plot the car with two trailers
//
x0=state(1);y0=state(2);theta0=state(3);theta1=state(4);
theta2=state(5);phi=state(6);
  theta = theta0 ; x=x0 ; y=y0 ;
// rear wheels
wheel1 =  [ -1/8 1/8 ;  1/6  1/6 ] ; 
wheel2 =  [ -1/8 1/8 ; -1/6 -1/6 ] ;
// front wheels
wheel3 = [
(1-cos(phi)/8)   (1+cos(phi)/8)
(1/6-sin(phi)/8) (1/6+sin(phi)/8) 
         ] ;
wheel4 = [
(1-cos(phi)/8)    (1+cos(phi)/8)
(-1/6-sin(phi)/8) (-1/6+sin(phi)/8) 
         ] ;
// 
// car + wheels
xy_car = [ [ -1/3   7/6   4/3  4/3  7/6  -1/3  -1/3
    -1/3  -1/3  -1/6  1/6  1/3   1/3  -1/3 ] wheel1 wheel2 wheel3 wheel4 ] ;
xy_car = diag([x,y])*ones(2,15) + ...
   bigL*[cos(theta0) -sin(theta0);sin(theta0) cos(theta0)]*xy_car  ;
//
//
//     Trailer 1
x = x - d1*cos(theta1) ;
y = y - d1*sin(theta1) ;

//
// shift + rotation
xy_T1 =diag([x,y])*ones(2,11) + ...
      [cos(theta1) -sin(theta1);sin(theta1) cos(theta1)]*xy_T1  ;

//
//     Trailer 2
x = x - d2*cos(theta2) ;
y = y - d2*sin(theta2) ;

//
// shift + rotation
xy_T2 = diag([x,y])*ones(2,11)+ ...
[cos(theta2) -sin(theta2);sin(theta2) cos(theta2)]*xy_T2  ;
//
// plots
//
//  Car 
xpoly(xy_car(1,1:7),xy_car(2,1:7),'lines') ;
//   The 4 wheels
xpolys(matrix(xy_car(1,8:15),2,4),matrix(xy_car(2,8:15),2,4),[1 1 1 1])

// trailer 1 
xpoly(xy_T1(1,1:5),xy_T1(2,1:5),'lines') ;
// hitch and wheels
xpolys(matrix(xy_T1(1,6:11),2,3),matrix(xy_T1(2,6:11),2,3),[1 1 1 1]);

//
// trailer 2 
xpoly(xy_T2(1,1:5),xy_T2(2,1:5),'lines') ;
// hitch and wheels
xpolys(matrix(xy_T2(1,6:11),2,3),matrix(xy_T2(2,6:11),2,3),[1 1 1 1]);
