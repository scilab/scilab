function demo_car()
  initial=[3;3;0;0]
  final=[0;0;0;0]
  f = gcf() ;
  f.pixmap = 'on' ;
  toolbar(0,'off');
  state=car_solve(initial,final);
  display_car_trajectory(state)
endfunction

function state=car_solve(initial,final)
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
// initial: initial position [x,y,theta,phi]
// final  :   final position [x,y,theta,phi]
//        theta  : the car angle
//        phi    : the front wheel angle
  
  bigT = 1 ;//basic time interval for one  smooth motion (s)
  bigL = 1 ;// car length (m) 

  // computation of  intermediate configuration 
  x0 = maxi(initial(1),final(2))   ....
       + bigL*abs(tan(initial(3))) ...
       + bigL*abs(tan(final(3))) ...
       + bigL*(abs(initial(2)-final(2))/bigL)^(1/2) ;
  y0 = (initial(2)+final(2))/2 ;
  intermediate=[x0,y0,0,0]'

  // first polynomial curve
  state=[matrix(initial,1,-1);
	 car_polynomial_curve(initial,intermediate,"direct")]
  //
  // second polynomial curve
  state = [ state; 
	    matrix(intermediate,1,-1) 
	    car_polynomial_curve(final,intermediate,"reverse")
	    matrix(final,1,-1)] 
endfunction

function state=car_polynomial_curve(initial,final,orient)

  nbpt = 40 ; //  sampling of motion 
  theta1 = initial(3) ; phi1 = initial(4) ;
  da = initial(1)-final(1)
  
  M = [da^3     da^4     da^5 
       3*da^2   4*da^3   5*da^4
       6*da    12*da^2  20*da^3 ] ;
  
  q = [initial(2)-final(2)
       tan(theta1) 
       tan(phi1)/(bigL*(cos(theta1)^3))] ;
  
  p = inv(M)*q ;
  tau=(0:nbpt)'/nbpt
  phi=tau.*tau.*(3-2*tau) ;
  if orient=='reverse' then
    a = (1-phi)*final(1) + phi*initial(1) ;
  else
    a = (1-phi)*initial(1) + phi*final(1) ;
  end
  da=a-final(1)
  
  f=  final(2)+ p(1).*da^3 +    p(2).*da^4 +    p(3).*da^5 ;
  df  =       3*p(1).*da^2 +  4*p(2).*da^3 +  5*p(3).*da^4 ;
  ddf =       6*p(1).*da   + 12*p(2).*da^2 + 20*p(3).*da^3 ;
  
  k = ddf ./ ((1+df.*df)^(3/2)) ;
  state=[ a   f  atan(df) atan(k*bigL)]
endfunction

function display_car_trajectory(state)
  bigL=1
  set figure_style new;xbasc();xselect()
  a=gca()
  drawlater()
  a.isoview="on"
  a.data_bounds=[mini(state(:,1))-0.5*bigL, mini(state(:,2))-1.5*bigL
		 maxi(state(:,1))+1.5*bigL, maxi(state(:,2))+1.5*bigL]
  rect=matrix(a.data_bounds',-1,1)
  xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
  C=build_car()
  Cinit=[];Cend=[];Cinter=[];
  for k=1:size(C,'*')
    Cinit=[Cinit copy(C(k))];
    Cinter=[Cinter,copy(C(k))];
    Cend=[Cend,copy(C(k))]
  end
  // starting configuration
  draw_car(Cinit,state(1,:))
  // end configuration
  draw_car(Cend,state($,:))
  // intermediate configuration (inversion of velocity)
  draw_car(Cinter,state(ceil(size(state,1)/2),:)) ;
  // trajectory of the linearizing output
  t1=polyline([state(1,1) state(1,2);state(1,1) state(1,2)]) ;
  t1.line_style=2;
  realtimeinit(0.1)
  for i=1:size(state,1)
    realtime(i) 
    drawlater()
    draw_car(C, state(i,:)) 
    t1.data=[t1.data;state(i,1) state(i,2)];
    drawnow()
  end 
  for i=(1:30)+size(state,1),realtime(i),end
  xdel()
endfunction


function C=build_car()
//build the graphic object for the car
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
  //return vector of handle on the objects
  C=[hcar,hwheel1,hwheel2,hwheel3,hwheel4]
endfunction

function draw_car(C,pos)
  drawlater()
  [x,y,theta,phi]=(pos(1),pos(2),pos(3),pos(4))
  bigL=1
  Rc=[cos(theta) sin(theta);-sin(theta) cos(theta)]
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
  drawnow()
  show_pixmap();
endfunction

function h=polyline(xy)
  xpoly(xy(:,1),xy(:,2),'lines')
  h=gce()
endfunction
