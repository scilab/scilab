function []=mvcr(x,y,theta,phi)
///////////////%% BEGIN OF SCRIPT-FILE mvcr %%%%%%%%%%%%%%%
//
//  CAR PACKING VIA FLATNESS AND FRENET FORMULAS
//
//    explicit computation and visualisation of the motions.
//
//    February 1993
//
// ............................................................
// :         pierre ROUCHON  <rouchon@cas.ensmp.fr>           :
// : Centre Automatique et Systemes, Ecole des Mines de Paris :
// : 60, bd Saint Michel -- 75272 PARIS CEDEX 06, France      :
// :    Telephone: (1) 40 51 91 15 --- Fax: (1) 43 54 18 93   :
// :..........................................................:
//
//
// bigL:  car length (m) 
// bigT: basic time interval for one  smooth motion (s)
// a0, a1, p(3): intermediate variables for polynomial 
//           curves definition
//
//
// Copyright INRIA
xbasc() ;
bigT = 1 ; bigL = 1 ;
a0 =0 ; a1 = 0 ;
p= [0 0 0 ] ;
//
// initial configuration of the car
x1 = x ; y1 = y ; theta1 = theta ; phi1 = phi ;
// final configuration of the car 
x2 = 0 ; y2 = 0  ; theta2 =0; phi2 = 0 ;
// Constraints: y1 > y2 and -%pi/2 < theta1,2, phi1,2 < %pi/2
//
//  sampling of motion 1 --> 0 and 0 --> 2 
     nbpt = 40 ;
// computation of  intermediate configuration 
x0 = maxi(x1,x2)   ....
    + bigL*abs(tan(theta1)) .....
    + bigL*abs(tan(theta2)) .....
    + bigL*(abs(y1-y2)/bigL)^(1/2) ;
y0 = (y1+y2)/2 ;
//
//
// first polynomial curve
a0 = x0 ; b0 = y0 ;
a1 = x1 ; b1 = y1 ; 
M = [ 
  (a1-a0)^3     (a1-a0)^4     (a1-a0)^5 
3*(a1-a0)^2   4*(a1-a0)^3   5*(a1-a0)^4
6*(a1-a0)    12*(a1-a0)^2  20*(a1-a0)^3 
      ] ;
q = [ 
   b1-b0 
   tan(theta1) 
   tan(phi1)/(bigL*(cos(theta1)^3)) 
    ] ;
p = inv(M)*q ;
//
// computation the first motion, time: 1 -> 0
 state=[ x1 y1 theta1 phi1 ] ;
 for i=1:(nbpt+1)
    tau = (i-1)/nbpt ;
    phi=tau*tau*(3-2*tau) ;
    a = (1-phi)*a1 + phi*a0 ;
    f=  b0+ p(1).*(a-a0)^3 +    p(2).*(a-a0)^4 +    p(3).*(a-a0)^5 ;
   df  = 3*p(1).*(a-a0)^2 +  4*p(2).*(a-a0)^3 +  5*p(3).*(a-a0)^4 ;
   ddf = 6*p(1).*(a-a0)   + 12*p(2).*(a-a0)^2 + 20*p(3).*(a-a0)^3 ;
   k = ddf / ((1+df*df)^(3/2)) ;
   state = [ state; a   f  atan(df) atan(k*bigL)] ; 
 end  
//
//
// second polynomial curve
a0 = x0 ; b0 = y0 ;
a1 = x2 ; b1 = y2 ; 
M = [ 
  (a1-a0)^3     (a1-a0)^4     (a1-a0)^5 
3*(a1-a0)^2   4*(a1-a0)^3   5*(a1-a0)^4
6*(a1-a0)    12*(a1-a0)^2  20*(a1-a0)^3 
      ] ;
q = [ 
      b1-b0 
   tan(theta2) 
   tan(phi2)/(bigL*(cos(theta2)^3)) 
    ] ;
p = inv(M)*q ;
//
// computation of the second motion 0 --> 2
 for i=1:(nbpt+1)
   tau = (i-1)/nbpt ;
   phi=tau*tau*(3-2*tau) ;
   a = (1-phi)*a0 + phi*a1 ;
   f=  b0+ p(1).*(a-a0)^3 +    p(2).*(a-a0)^4 +    p(3).*(a-a0)^5 ;
   df  = 3*p(1).*(a-a0)^2 +  4*p(2).*(a-a0)^3 +  5*p(3).*(a-a0)^4 ;
   ddf = 6*p(1).*(a-a0)   + 12*p(2).*(a-a0)^2 + 20*p(3).*(a-a0)^3 ;
   k = ddf / ((1+df*df)^(3/2)) ;
   state = [ state; a   f  atan(df) atan(k*bigL)] ;
 end  
//
//
// Graphics 
//
// window size
xmini = mini(state(:,1))-0.5*bigL ;
xmaxi = maxi(state(:,1))+1.5*bigL ;
ymini = mini(state(:,2))-1.5*bigL ;
ymaxi = maxi(state(:,2))+1.5*bigL ;

//xsetech([0,0,1,1],[xmini,ymini,xmaxi,ymaxi]);
pixb=xget("pixmap");xset("pixmap",1);xset("wwpc");
isoview(xmini,xmaxi,ymini,ymaxi)
rect=[xmini ymini xmaxi ymaxi]
xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
// starting configuration
ptcr([x1,y1,theta1,phi1]) ;
// end configuration
ptcr([x2,y2,theta2,phi2]) ;
// intermediate configuration (inversion of velocity)
ptcr([x0,y0,0,0]) ;
// trajectory of the linearizing output
xpoly(state(:,1),state(:,2),'lines') ;
if xget('pixmap')==1 then xset("wshow");end
// movies 
[n m] = size(state) ;
if driver()<>'Pos' then
  if xget('pixmap')==0 then
    xset('alufunction',6);...
	for i=1:n,
      ptcr( state(i,:)) ; ptcr( state(i,:)) ;
    end ;
    xset('alufunction',3);
  else
    xset('alufunction',6);
    for i=1:n,
      ptcr( state(i,:)) ; 
      xset("wshow");
      ptcr( state(i,:))
    end ;  
    xset('alufunction',3)
    ptcr( state(n,:)) ; 
    xset("wshow");
  end
else //used for gif animation generation
  for i=1:2:n,
    xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
    ptcr([x1,y1,theta1,phi1]) ;
    ptcr([x2,y2,theta2,phi2]) ;
    ptcr([x0,y0,0,0]) 
    xpoly(state(:,1),state(:,2),'lines') ;
    ptcr( state(i,:)) ; 
    xset("wshow");
  end ;  
end
xset("pixmap",pixb)


function []=mvcr2T(x,y,theta1,theta2,theta3,phi)
xbasc();
//
//  CAR WITH 2 TRAILERS PACKING VIA FLATNESS AND FRENET FORMULAS
//
//   explicit computation and visualisation of the motions.
//
//    February 1993
//
// ............................................................
// :         pierre ROUCHON  <rouchon@cas.ensmp.fr>           :
// : Centre Automatique et Systemes, Ecole des Mines de Paris :
// : 60, bd Saint Michel -- 75272 PARIS CEDEX 06, France      :
// :    Telephone: (1) 40 51 91 15 --- Fax: (1) 43 54 18 93   :
// :..........................................................:
//
// lengths 
//    bigL:  car length (m) 
//   d1: trailer 1 length (m)
//   d2: trailer 2 length (m)
// bigT: basic time interval for one  smooth motion (s)
// a0, a1, b0, p(5): intermediate variables for polynomial 
//           curves definition
//
bigT = 1 ; 
bigL = 1 ; d1 = 1.5 ; d2 = 1 ; 
a0 =0 ; a1 = 0 ; b0 = 0 ;
p= [0 0 0 0 0 ] ;
//
// initial configuration
//   the system is described via the coordinates of last trailer
//    
x2_1 = x ; y2_1 = y ; 
theta2_1= theta1; theta12_1 = theta2 ; theta01_1= theta3 ;
phi_1 = phi ;
//
// final configuration of the car 
x2_2 = 0 ; y2_2 = 0  ;
theta2_2= 0 ; theta12_2 = 0 ; theta01_2= 0 ;
                                   phi_2 = 0 ;
//
// sampling of motion 1 --> 0 and of motion 0 --> 2
        nbpt1 = 40 ; nbpt2 = 40 ;
//
// Constraints: y2_1 > y2_2 and 
// the 4 angles  theta2_1,2
//               theta12_1,2
//               theta01_1,2
//               phi_1,2 
//   must belong to  ] -%pi/2 , + %pi/2 [
//               
//
// conputation of  intermediate configuration
LL=bigL+d1+d2
x2_0 = maxi(x2_1,x2_2) ....
      + LL*abs(tan(theta2_1)) ....
      + LL*abs(tan(theta12_1)) .... 
      + LL*abs(tan(theta01_1)) .... 
      + LL*( abs(y2_1-y2_2)/(d1+d2+bigL) )^(1/2) ;
y2_0 = (y2_1+y2_2)/2 ;
//
//
// first polynomial curve
a0 = x2_0 ; b0 = y2_0 ;
a1 = x2_1 ; b1 = y2_1 ; 
p=cr2Tkf((b1-b0),theta2_1,theta12_1,theta01_1,phi_1) ;
//
// computation the first motion  1 -> 0
 theta2 = theta2_1 ;
 theta1 = theta12_1+theta2 ;
 theta0 = theta01_1+theta1 ;
 phi = phi_1 ;
 x0=x2_1+d2*cos(theta2)+d1*cos(theta1) ;
 y0=y2_1+d2*sin(theta2)+d1*sin(theta1) ;
 state_1 = [x0 y0 theta0 theta1 theta2 phi] ;
 for i=1:(nbpt1+1)
   tau = (i-1)/nbpt1 ;
   phi=tau*tau*(3-2*tau) ;
   aa = (1-phi)*a1 + phi*a0 ;
   [bb df d2f d3f d4f d5f] = cr2Tfjt(aa) ;
   [k2 k1 k0 dk0]=cr2Tfk(df,d2f,d3f,d4f,d5f) ;
   theta2 = atan(df);
   theta1 = atan(k2*d2)+theta2;
   theta0 = atan(k1*d1) + theta1 ;
   phi = atan(k0*bigL) ;
   x0=aa+d2*cos(theta2)+d1*cos(theta1) ;
   y0=bb+d2*sin(theta2)+d1*sin(theta1) ;
   state_1 = [ state_1 ; x0 y0 theta0 theta1 theta2 phi] ;
 end ;
//
// second polynomial curve
a0 = x2_0 ; b0 = y2_0 ;
a1 = x2_2 ; b1 = y2_2 ; 
 p=cr2Tkf((b1-b0),theta2_2,theta12_2,theta01_2,phi_2) ;
//
// computation of the second motion  0 -> 2
 theta2 = 0 ;
 theta1 = 0 ;
 theta0 = 0 ;
 phi = 0 ;
 x0=x2_0+d2*cos(theta2)+d1*cos(theta1) ;
 y0=y2_0+d2*sin(theta2)+d1*sin(theta1) ;
 state_2 = [x0 y0 theta0 theta1 theta2 phi] ;
 for i=1:(nbpt2+1)
   tau = (i-1)/nbpt2 ;
   phi=tau*tau*(3-2*tau) ;
   aa = (1-phi)*a0 + phi*a1 ;
   [bb df d2f d3f d4f d5f] = cr2Tfjt(aa) ;
   [k2 k1 k0 dk0]=cr2Tfk(df,d2f,d3f,d4f,d5f) ;
   theta2 = atan(df);
   theta1 = atan(k2*d2)+theta2;
   theta0 = atan(k1*d1) + theta1 ;
   phi = atan(k0*bigL) ;
   x0=aa+d2*cos(theta2)+d1*cos(theta1) ;
   y0=bb+d2*sin(theta2)+d1*sin(theta1) ;
   state_2 = [ state_2 ; x0 y0 theta0 theta1 theta2 phi] ;
 end ;
//
// Graphics 
//
// window size
xmini = mini([mini(state_1(:,1)) mini(state_2(:,1))]) -1.5*(d1+d2)  ;
xmaxi = maxi([maxi(state_1(:,1)) maxi(state_1(:,1))]) +1.5*bigL ;
ymini = mini([mini(state_1(:,2)) mini(state_2(:,2))])-bigL;
ymaxi = maxi([maxi(state_1(:,2)) maxi(state_1(:,2))])+bigL;
rect=[xmini ymini xmaxi ymaxi]
pixb=xget("pixmap");xset("pixmap",1);xset("wwpc");
xsetech([0,0,1,1],rect);
isoview(xmini,xmaxi,ymini,ymaxi)
xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
//

xy_T1 = [ [-bigL/3  bigL/3   bigL/3  -bigL/3  -bigL/3  
-bigL/3 -bigL/3 bigL/3  bigL/3  -bigL/3 ], .....
 [ bigL/3  d1;
  0     0], .....
 [-bigL/8   bigL/8 
 bigL/6  bigL/6 
 ], .... 
 [-bigL/8   bigL/8 
-bigL/6 -bigL/6  ] ] ;

xy_T2 = [[-bigL/3  bigL/3   bigL/3  -bigL/3  -bigL/3  
-bigL/3 -bigL/3 bigL/3  bigL/3  -bigL/3 ],...
 [bigL/3  d2
  0         0 ],[ -bigL/8   bigL/8 
 bigL/6  bigL/6 
 ],[ -bigL/8   bigL/8 
-bigL/6 -bigL/6 ] ] ;

// starting configuration
   x2=x2_1 ; y2=y2_1 ;
   theta2 = theta2_1 ;
   theta1 = theta12_1+theta2 ;
   theta0 = theta01_1+theta1 ;
   phi = phi_1 ;
   x1=x2+d2*cos(theta2) ;
   y1=y2+d2*sin(theta2) ;
   x0=x1+d1*cos(theta1) ;
   y0=y1+d1*sin(theta1) ;
   ptsts=[x0,y0,theta0,theta1,theta2,phi] ;
ptcr2T(ptsts) ;
// end configuration
   x2=x2_2 ; y2=y2_2 ;
   theta2 = theta2_2 ;
   theta1 = theta12_2+theta2 ;
   theta0 = theta01_2+theta1 ;
   phi = phi_2 ;
   x1=x2+d2*cos(theta2) ;
   y1=y2+d2*sin(theta2) ;
   x0=x1+d1*cos(theta1) ;
   y0=y1+d1*sin(theta1) ;
   ptste=[x0,y0,theta0,theta1,theta2,phi] 
ptcr2T(ptste) ;
// intermediate configuration (inversion of velocity)
   x2=x2_0 ; y2=y2_0 ;
   theta2 = 0 ;
   theta1 = 0;
   theta0 = 0 ;
   phi = 0;
   x1=x2+d2*cos(theta2) ;
   y1=y2+d2*sin(theta2) ;
   x0=x1+d1*cos(theta1) ;
   y0=y1+d1*sin(theta1) ;
   ptsti=[x0,y0,theta0,theta1,theta2,phi]    
ptcr2T(ptsti) ;
state_1 =[state_1;state_2] ;
x_lin = state_1(:,1)-d1*cos(state_1(:,4))-d2*cos(state_1(:,5)) ;
y_lin = state_1(:,2)-d1*sin(state_1(:,4))-d2*sin(state_1(:,5)) ;
//  motion
//
// trajectory of the linearizing output
xpoly(x_lin,y_lin,'lines')
if xget('pixmap')==1 then xset("wshow");end

// movies 
[n,m] = size(state_1) ;
if driver()<>'Pos' then
  if xget('pixmap')==0 then
    xset('alufunction',6);
    for j=1:n
      ptcr2T(state_1(j,:));ptcr2T(state_1(j,:));
    end ;
    xset('alufunction',3);
  else
    xset('alufunction',6);
    for j=1:n
      ptcr2T(state_1(j,:));
      xset("wshow");
      ptcr2T(state_1(j,:));
    end 
    xset('alufunction',3);
    ptcr2T(state_1(n,:));
    xset("wshow");
  end
else //only use for gif animation generation
  for j=1:4:n
    xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
    ptcr2T(ptsts) ;
    ptcr2T(ptsti) ;
    ptcr2T(ptste) ;
    xpoly(x_lin,y_lin,'lines')
    ptcr2T(state_1(j,:))
    xset("wshow");
  end 
end

xset("pixmap",pixb)
////%%%%%%%%%%%% END OF SCRIPT-FILE mvcr2T %%%%%%%%%%%%%

function []=dbcr()
//
//  CAR PACKING VIA FLATNESS AND FRENET FORMULAS
//
//   debugg and verification via integration 
//        of the non holonomic system
//
//                   February 1993
//
// ............................................................
// :         pierre ROUCHON  <rouchon@cas.ensmp.fr>           :
// : Centre Automatique et Systemes, Ecole des Mines de Paris :
// : 60, bd Saint Michel -- 75272 PARIS CEDEX 06, France      :
// :    Telephone: (1) 40 51 91 15 --- Fax: (1) 43 54 18 93   :
// :..........................................................:
//
//
// bigL:  car length (m) 
// bigT: basic time interval for one  smooth motion (s)
// a0, a1, p(3): intermediate variables for polynomial 
//           curves definition
//
//
bigT = 1 ; bigL = 1 ;
a0 =0 ; a1 = 0 ;
p= [0 0 0 ] ;
//
// initial configuration of the car
x1 = 0 ; y1 = 4 ; theta1 = %pi/2.5 ; phi1 = 0 ;
// final configuration of the car 
x2 = 0 ; y2 = 0  ; theta2 =0; phi2 = 0 ;
// Constraints: y1 > y2 and -%pi/2 < theta1,2, phi1,2 < %pi/2
//
// conputation of  intermediate configuration 
x0 = maxi(x1,x2)   ....
    + bigL*abs(tan(theta1)) .....
    + bigL*abs(tan(theta2)) .....
    + bigL*(abs(y1-y2)/bigL)^(1/2) ;
y0 = (y1+y2)/2 ;
//
//
// first polynomial curve
a0 = x0 ; b0 = y0 ;
a1 = x1 ; b1 = y1 ; 
M = [ 
  (a1-a0)^3     (a1-a0)^4     (a1-a0)^5 
3*(a1-a0)^2   4*(a1-a0)^3   5*(a1-a0)^4
6*(a1-a0)    12*(a1-a0)^2  20*(a1-a0)^3 
      ] ;
q = [ 
   b1-b0 
   tan(theta1) 
   tan(phi1)/(bigL*(cos(theta1)^3)) 
    ] ;
p = inv(M)*q ;
//
// simulation of the first motion, time: 0 -> bigT
 [t_1,state_1]=ode23('car',0,bigT, [ x1 y1 theta1 phi1 ]);
//
//
// second polynomial curve
a0 = x0 ; b0 = y0 ;
a1 = x2 ; b1 = y2 ; 
M = [ 
  (a1-a0)^3     (a1-a0)^4     (a1-a0)^5 
3*(a1-a0)^2   4*(a1-a0)^3   5*(a1-a0)^4
6*(a1-a0)    12*(a1-a0)^2  20*(a1-a0)^3 
      ] ;
q = [ 
      b1-b0 
   tan(theta2) 
   tan(phi2)/(bigL*(cos(theta2)^3)) 
    ] ;
p = inv(M)*q ;
//
// simulation of the second motion, time: bigT -> 2bigT
 [n m]=size(t_1);
 [t_2,state_2]=ode23('car',bigT,2*bigT, state_1(n,:) );
//
//
// result array merging
 t_1=t_1(2:n) ; state_1=state_1(2:n,:);
 t=[
    t_1 
    t_2
    ]; 
state = [
         state_1 
         state_2 
        ];
//
//
 plot(t,state) ;
// xlabel('time (s)') ; 
// ylabel('x y theta phi ') ;
//%%%%%%%%%//%%%% END OF SCRIPT-FILE dbcr  %%%%%%%%%%%%


function []=dbcr2T()
//
//  CAR WITH 2 TRAILERS PACKING VIA FLATNESS AND FRENET FORMULAS
//  
//  debugg and verification via the integration 
//        of the non holonomic system.
//
//    February 1993
//
// ............................................................
// :         pierre ROUCHON  <rouchon@cas.ensmp.fr>           :
// : Centre Automatique et Systemes, Ecole des Mines de Paris :
// : 60, bd Saint Michel -- 75272 PARIS CEDEX 06, France      :
// :    Telephone: (1) 40 51 91 15 --- Fax: (1) 43 54 18 93   :
// :..........................................................:
//
// lengths 
//    bigL:  car length (m) 
//   d1: trailer 1 length (m)
//   d2: trailer 2 length (m)
// bigT: basic time interval for one  smooth motion (s)
// a0, a1, p(5): intermediate variables for polynomial 
//           curves definition
//
bigT = 1 ; 
bigL = 1 ; d1 = 1.5 ; d2 = 1 ; 
a0 =0 ; a1 = 0 ; b0 = 0 ;
p= [0 0 0 0 0 ] ;
//
// initial configuration
//   the system is described via the coordinates of last trailer
x2_1 = 0 ; y2_1 = 6 ; 
theta2_1= %pi/8; theta12_1 =  %pi/8 ; theta01_1= %pi/8 ;
                                  phi_1 = %pi/8 ;
//
// final configuration of the car 
x2_2 = 0 ; y2_2 = 0  ;
theta2_2= 0 ; theta12_2 = 0 ; theta01_2= 0 ;
                                   phi_2 = 0 ;
//
// Constraints: y2_1 > y2_2 and 
// the 4 angles  theta2_1,2
//               theta12_1,2
//               theta01_1,2
//               phi_1,2 
//   must belong to  ] -%pi/2 , + %pi/2 [
//               
//
// conputation of  intermediate configuration
LL=bigL+d1+d2 ;
x2_0 = maxi(x2_1,x2_2) ....
      + LL*abs(tan(theta2_1)) ....
      + LL*abs(tan(theta12_1)) .... 
      + LL*abs(tan(theta01_1)) .... 
      + LL*( abs(y2_1-y2_2)/(d1+d2+bigL) )^(1/2) ;
y2_0 = (y2_1+y2_2)/2 ;
//
//
//
// first polynomial curve
a0 = x2_0 ; b0 = y2_0 ;
a1 = x2_1 ; b1 = y2_1 ; 
p=cr2Tkf((b1-b0),theta2_1,theta12_1,theta01_1,phi_1) ;
//
// simulation of the first motion  0 -> T
//   time t between 0 and bigT
 theta2 = theta2_1 ;
 theta1 = theta12_1+theta2 ;
 theta0 = theta01_1+theta1 ;
 phi = phi_1 ;
 x0=x2_1+d2*cos(theta2)+d1*cos(theta1) ;
 y0=y2_1+d2*sin(theta2)+d1*sin(theta1) ;
 [t_1,state_1]=ode45('car2T',0,bigT, ....
                     [ x0 y0 theta0 theta1 theta2 phi ]);
// graphics 
 subplot(121);
   plot(t_1,state_1(:,1:2)) ;
   xlabel('time (s)') ; 
   ylabel('x_car y_car (m)') ;
 subplot(122);
   plot(t_1,state_1(:,3:6)) ;
   xlabel('time (s)') ; 
   ylabel('theta0 theta1 theta2 phi (rd)') ;
//
//
// second polynomial curve
a0 = x2_0 ; b0 = y2_0 ;
a1 = x2_2 ; b1 = y2_2 ; 
 p=cr2Tkf((b1-b0),theta2_2,theta12_2,theta01_2,phi_2) ;
//
// simulation of the second motion  bigT -> 2*bigT
//
// important remark: due to numerical instability of the  
//  integration during inverse motion, we integrate
//  from the final position 2 to the intermediate position 0.
//
 theta2 = theta2_2 ;
 theta1 = theta12_2+theta2 ;
 theta0 = theta01_2+theta1 ;
 phi = phi_2 ;
 x0=x2_2+d2*cos(theta2)+d1*cos(theta1) ;
 y0=y2_2+d2*sin(theta2)+d1*sin(theta1) ;
 [t_2,state_2]=ode45('car2T',0,bigT, ....
                     [ x0 y0 theta0 theta1 theta2 phi ]);
//
//
//
// graphics 
  t_2 = 2*bigT - t_2 ;
 subplot(121);
   plot(t_2,state_2(:,1:2)) ;
   xlabel('time (s)') ; 
   ylabel('x_car y_car (m)') ;
 subplot(122);
   plot(t_2,state_2(:,3:6)) ;
   xlabel('time (s)') ; 
   ylabel('theta0 theta1 theta2 phi (rd)') ;
//%%%%%%%%%%%%%% END OF SCRIPT-FILE dbcr2T  %%%%%%%%%%%%
