function demo_truck()
  initial = [-2;3;0.5235988;0;0;1]
  final   = [0;0;0;0;0;0]
  state = truck_solve(initial,final);
  f = gcf() ;
  f.pixmap = 'on' ;
  toolbar(0,'off') ;
  display_truck_trajectory(state)
endfunction

function state=truck_solve(initial,final)
//
//  CAR  WITH 2 TRAILERS PACKING VIA FLATNESS AND FRENET FORMULAS
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
// initial: initial position [x,y,theta1,theta2,theta3,phi]
// final  :   final position [x,y,theta1,theta2,theta3,phi]
//        theta1,theta2,theta3  : the car and the trailers angles
//        phi    : the front wheel angle
  
  bigT = 1 ;//basic time interval for one  smooth motion (s)
  bigL = 1 ;// car length (m) 
  d1 = 1.5 ; d2 = 1 ; //trailers length

  // computation of  intermediate configuration 
  LL=bigL+d1+d2
  x0 = maxi(initial(1),final(2))   ....
       + LL*abs(tan(initial(3))) ...
       + LL*abs(tan(initial(4))) ...
       + LL*abs(tan(initial(5))) ...
       + LL*(abs(initial(2)-final(2))/(d1+d2+bigL))^(1/2) ;
  y0 = (initial(2)+final(2))/2 ;
  intermediate=[x0,y0,0,0,0,0]'

  // first polynomial curve
  state=truck_polynomial_curve(initial,intermediate,"direct")
  //
  // second polynomial curve
  state = [ state;
	    truck_polynomial_curve(final,intermediate,"reverse")	    ]
	   
endfunction



function state=truck_polynomial_curve(initial,final,orient)

  nbpt = 40 ; //  sampling of motion 
  phi = initial(6) ;

  theta2 = initial(3) ; 
  theta1 = initial(4)+theta2
  theta0 = initial(5)+theta1
  
  x0=initial(1)+d2*cos(theta2)+d1*cos(theta1) ;
  y0=initial(2)+d2*sin(theta2)+d1*sin(theta1) ;
  if orient<>'reverse' then
    state = [x0 y0 theta0 theta1 theta2 phi] ;
  else
    state=[]
  end
  a0=final(1);a1=initial(1);b0=final(2)
  db = initial(2)-final(2)
  p = cr2Tkf(db,initial(3),initial(4),initial(5),phi) ;

  tau=(0:nbpt)'/nbpt
  phi=tau.*tau.*(3-2*tau) ;
  if orient=='reverse' then
    aa = (1-phi)*final(1) + phi*initial(1) ;
  else
    aa = (1-phi)*initial(1) + phi*final(1) ;
  end
  for i=1:(nbpt+1)
    [bb,df,d2f,d3f,d4f,d5f] = cr2Tfjt(aa(i)) ;
    [k2,k1,k0,dk0]=cr2Tfk(df,d2f,d3f,d4f,d5f) ;
    theta2 = atan(df);
    theta1 = atan(k2*d2)+theta2;
    theta0 = atan(k1*d1) + theta1 ;
    phi = atan(k0*bigL) ;
    x0=aa(i)+d2*cos(theta2)+d1*cos(theta1) ;
    y0=bb+d2*sin(theta2)+d1*sin(theta1) ;
    state= [ state; 
	     x0 y0 theta0 theta1 theta2 phi] ;
  end
endfunction

function [ff,df,d2f,d3f,d4f,d5f]=cr2Tfjt(a)
//
//
  da = a-a0
  M= [  da^5      da^6      da^7      da^8       da^9 
      5*da^4    6*da^5    7*da^6    8*da^7     9*da^8 
     20*da^3   30*da^4   42*da^5   56*da^6    72*da^7 
     60*da^2  120*da^3  210*da^4  336*da^5   504*da^6 
    120*da^1  360*da^2  840*da^3 1680*da^4  3024*da^5 
    120       720*da^1 2520*da^2 6720*da^3 15120*da^4]*p ;
  ff  = M(1) + b0 ;
  df  = M(2) ;
  d2f = M(3) ;
  d3f = M(4) ;
  d4f = M(5) ;
  d5f = M(6) ;
endfunction

function coef=cr2Tkf(b,theta2,theta12,theta01,phi)
//
//
  da = a1-a0
M = [1*da^5    1*da^6   1*da^7    1*da^8    1*da^9 
     5*da^4    6*da^5   7*da^6    8*da^7    9*da^8 
    20*da^3   30*da^4  42*da^5   56*da^6   72*da^7 
    60*da^2  120*da^3 210*da^4  336*da^5  504*da^6 
   120*da^1  360*da^2 840*da^3 1680*da^4 3024*da^5] ;
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
endfunction

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

endfunction

function display_truck_trajectory(state)
   bigL = 1 ; d1 = 1.5 ; d2 = 1;
  set figure_style new;xbasc(),xselect()
  a=gca()
  drawlater()
  a.isoview="on"
  a.data_bounds=[mini(state(:,1))-1.5*(d1+d2), mini(state(:,2))-bigL
		 maxi(state(:,1))+1.5*bigL, maxi(state(:,2))+bigL]
  rect=matrix(a.data_bounds',-1,1)
  xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
  C=build_truck()
  Cinit=[];Cend=[];Cinter=[];
  for k=1:size(C,'*')
    Cinit=[Cinit copy(C(k))];
    Cinter=[Cinter,copy(C(k))];
    Cend=[Cend,copy(C(k))]
  end
  // starting configuration
  draw_truck(Cinit,state(1,:))
  // end configuration
  draw_truck(Cend,state($,:))
  // intermediate configuration (inversion of velocity)
  draw_truck(Cinter,state(ceil(size(state,1)/2),:)) ;
  // trajectory of the linearizing output
  x_lin = state(:,1)-d1*cos(state(:,4))-d2*cos(state(:,5)) ;
  y_lin = state(:,2)-d1*sin(state(:,4))-d2*sin(state(:,5)) ;

  t1=polyline([x_lin(1) y_lin(1);x_lin(1) y_lin(1)]) ;
  t2=polyline([state(1,1) state(1,2);state(1,1) state(1,2)]) ;
  
  t1.line_style=2;
  t2.line_style=2;t2.foreground=5
  realtimeinit(0.2)
  for i=1:size(state,1)
    realtime(i) 
    drawlater()
    draw_truck(C, state(i,:)) 
    t1.data=[t1.data;x_lin(i), y_lin(i)];
    t2.data=[t2.data;state(i,1), state(i,2)];
    drawnow()
  end 
  for i=(1:30)+size(state,1),realtime(i),end
  xdel()
endfunction


function C=build_truck()
//build the graphic object for the truck
// 
  //the car
  hcar=polyline([-2,7,8,8,7,-2,-2;-2,-2,-1,1,2,2,-2]'/6)
  hcar.foreground=2
  // rear wheels
  hwheel1=polyline([[-1 1]/8; [1 1]/6]')
  hwheel1.thickness=2
   
  hwheel2=polyline([[-1 1]/8; -[1 1]/6]')
  hwheel2.thickness=2
  
  // front wheels
  hwheel3=polyline([[7 9]/8;[1 1]/6]')
  hwheel3.thickness=2
  hwheel4=polyline([[7 9]/8;-[1 1]/6]') 
  hwheel4.thickness=2
  
  //Trailer 1

  ht1=polyline([-1,1,1,-1,-1;-1,-1,1,1,-1]'*bigL/3)
  ht1.foreground=2
  hwheel5=polyline([[-1 1]/8;[1 1]/6]'*bigL)
  hwheel5.thickness=2
  hwheel6=polyline([[-1 1]/8;-[1 1]/6]'*bigL)
  hwheel6.thickness=2
  hhitch1=polyline([bigL/3  d1;0 0]')
  hhitch1.foreground=2
  //Trailer 2
  ht2=polyline([-1,1,1,-1,-1;-1,-1,1,1,-1]'*bigL/3)
  ht2.foreground=2
  hwheel7=polyline([[-1 1]/8;[1 1]/6]'*bigL)
  hwheel7.thickness=2
  hwheel8=polyline([[-1 1]/8;-[1 1]/6]'*bigL)
  hwheel8.thickness=2
  hhitch2=polyline([bigL/3  d2;0 0]')
  hhitch2.foreground=2



  //return vector of handle on the objects
  C=[hcar,hwheel1,hwheel2,hwheel3,hwheel4,..
     ht1,hwheel5, hwheel6,hhitch1,..
     ht2, hwheel7,hwheel8,hhitch2]
endfunction

function draw_truck(C,pos)
  drawlater()
  [x,y,theta1,theta2,theta3,phi]=(pos(1),pos(2),pos(3),pos(4),pos(5),pos(6))
  bigL = 1 ; d1 = 1.5 ; d2 = 1;
  Rc=[cos(theta1) sin(theta1);-sin(theta1) cos(theta1)]
  // the car
  xy = [-2,-2;7,-2;8,-1;8,1;7,2;-2,2;-2,-2]/6
  C(1).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  // rear wheels
  xy=[[-1 1]/8; [1 1]/6]'
  C(2).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  xy=[[-1 1]/8; -[1 1]/6]'
  C(3).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  // front wheels
  xy=[(1-cos(phi)/8) (1/6-sin(phi)/8)
      (1+cos(phi)/8) (1/6+sin(phi)/8)]
  C(4).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  xy=[(1-cos(phi)/8) (-1/6-sin(phi)/8)
      (1+cos(phi)/8) (-1/6+sin(phi)/8)]
  C(5).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  
  //Trailer 1
  Rc=[cos(theta2) sin(theta2);-sin(theta2) cos(theta2)]
  x = x - d1*cos(theta2) ;
  y = y - d1*sin(theta2) ;
  xy = [-1,1,1,-1,-1;-1,-1,1,1,-1]'*bigL/3;
  C(6).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  //wheels
  xy=[[-1 1]/8; [1 1]/6]'
  C(7).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  xy=[[-1 1]/8; -[1 1]/6]'
  C(8).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  //hitch
  xy=[bigL/3  d1;0 0]'
  C(9).data=ones(xy)*diag([x;y])+bigL*xy*Rc

  //Trailer 2
  Rc=[cos(theta3) sin(theta3);-sin(theta3) cos(theta3)]
  x = x - d2*cos(theta3) ;
  y = y - d2*sin(theta3) ;
  xy = [-1,1,1,-1,-1;-1,-1,1,1,-1]'*bigL/3;
  C(10).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  //wheels
  xy=[[-1 1]/8; [1 1]/6]'
  C(11).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  xy=[[-1 1]/8; -[1 1]/6]'
  C(12).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  //hitch
  xy=[bigL/3  d2;0 0]'
  C(13).data=ones(xy)*diag([x;y])+bigL*xy*Rc
  drawnow()
  show_pixmap();
endfunction

function h=polyline(xy)
  xpoly(xy(:,1),xy(:,2),'lines')
  h=gce()
endfunction
