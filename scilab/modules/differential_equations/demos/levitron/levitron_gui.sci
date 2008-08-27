//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

//The graphics objects builder and subsidiaries
//--------------------------------------------
function [H]=build_levitron(h)
//construct the graphical object representing the levitron in 3D

  n=21;// assumed to be odd
  [X,Y,Z]=levitron_facets(levitron_profile,n)
  //
  N=n-1;
  np=1+size(X,2)/N
  drawlater()
  f=gcf();f.color_map=graycolormap(N);
  a=gca();
  
  s=[((N/2)):(N-1),N:-1:((N/2)+1)];
  clf(),plot3d(X,Y,list(Z,ones(1,np-1).*.s))// the top

  H=gce();
  H.hiddencolor=-1;
  H.color_mode = -1;
  H.color_flag = 3;
  H.clip_state = 'off';

  H.user_data=[0 0 0 0 0 0];
  //The z axis
  x=0:0.05:2.5;
  param3d(zeros(x),zeros(x),x) //z axis
  e=gce();e.thickness=2;
  //the bottom
  plot3d([-1 1]*1.5,[-1 1]*1.5,zeros(2,2))
  e=gce();e.color_mode=color('red');
  //set axes properties
  a.clip_state='off';
  a.data_bounds=[-1 -1 0;1 1 2];
  a.axes_bounds=[0 0 0.8 1];
  a.axes_visible='off';a.box='off';
  a.cube_scaling="on";
  a.isoview="on";
  a.rotation_angles=[76 45];
  a.foreground=N
  a.x_label.text=''
  a.y_label.text=''
  a.z_label.text=''
  drawnow()
endfunction

function [zp,ro]=levitron_profile()
//zp : elevation of the profile's points 
//ro : radius
//c  : color
  zp = [0.6;0.22;0.15;0.06;0.04;0.02;-0.04;-0.06;
	-0.07;-0.15;-0.19;-0.27;-0.34;-0.38]*0.8;
  ro = [0;0.03;0.07;0.42;0.43;0.44;0.44;
	0.43;0.42;0.07;0.05;0.03;0.02;0];
  c = ones(ro)
endfunction

function [X,Y,Z]=levitron_facets(prof,n)
//computes the facets of the surface of a revolution surface
// prof is a function that returns the elevation and the radiud of ech
// profile point
// n: is the number of points for the rotation angle discretization

  [zp,ro]=prof();//one profile
  np=size(ro,'*');
  //generate points on the top surface
  t=linspace(0,2*%pi,n);
  x=ro*cos(t);//np x n
  y=ro*sin(t);
  z=zp*ones(t);
  
  //build the facets
  Iz=([0;0;1;1]*ones(1,np-1)+ones(4,1)*(1:(np-1))).*.ones(1,n-1);
  It=ones(1,np-1).*.([0;1;1;0]*ones(1,n-1)+ones(4,1)*(0:(n-2)));
  I=matrix(Iz+np*It,-1,1);
  X=matrix(x(I),4,-1);
  Y=matrix(y(I),4,-1);
  Z=matrix(z(I),4,-1);
endfunction


// The gui builder
//----------------
function levitron_gui()
  global levitron_controls
  fig=gcf();
  w=fig.axes_size;
  m=20;
  lslider=150;h=20;
  lv=40;
  xs=w(1)-lslider-lv-m-10;
  xv=xs+lslider+10;
  SliderStep=[0.001 0.001]
  
  yh=w(2)-(m+h);
  Height_title = uicontrol( ...
      "parent"              , fig,...
      "style"               , "text",...
      "string"              , "Height",...
      "units"               , "pixels",...
      "position"            , [xs yh+h lslider h],...
      "fontunits"           , "points",...
      "fontsize"            , 9,...
      "horizontalalignment" , "center", ...
      "background"          , [1 1 1], ...
      "tag"                 , "HeightTitle" ...
      );

  
  Slider_Height=uicontrol("parent",fig,..
	      "style","slider",..
	      "Min",0,..
	      "Max",100,..
	      "Value",1+100*0.72,..
	      "units","pixels",..
              "position"            , [xs yh lslider h],...
	      "tag"                 , "Height", ...
	      "callback"            , "update_height()" ...
	      );
  Value_Height = uicontrol("parent",fig,..
			   "Position"  , [xv yh lv h],...
			   "Style"     , "text",...
			   "String"    , "70",...
			   "BackgroundColor",[1 1 1]);
  
  yt=yh-(m+h);
  Theta_title = uicontrol( ...
      "parent"              , fig,...
      "style"               , "text",...
      "string"              , "Theta",...
      "units"               , "pixels",...
      "position"            , [xs yt+h lslider h],...
      "fontunits"           , "points",...
      "fontsize"            , 9,...
      "horizontalalignment" , "center", ...
      "background"          , [1 1 1], ...
      "tag"                 , "ThetaTitle" ...
      );

  Slider_Theta=uicontrol("parent",fig,..
			 "style","slider",..
			 "Min",0,..
			 "Max",100,..
			 "Value",70,..
			 "units","pixels",..
			 "position"            , [xs yt lslider h],...
			 "tag"                 , "Theta", ...
			 "callback"            , "update_theta()" ...
			 );
  Value_Theta = uicontrol("parent",fig,..
			   "Position"  , [xv yt lv h],...
			   "Style"     , "text",...
			   "String"    , "70",...
			   "BackgroundColor",[1 1 1]);
  yphi=yt-(m+h);
  Phi_title = uicontrol( ...
      "parent"              , fig,...
      "style"               , "text",...
      "string"              , "Phi",...
      "units"               , "pixels",...
      "position"            , [xs yphi+h lslider h],...
      "fontunits"           , "points",...
      "fontsize"            , 9,...
      "horizontalalignment" , "center", ...
      "background"          , [1 1 1], ...
      "tag"                 , "PhiTitle" ...
      );

  Slider_Phi=uicontrol("parent",fig,..
		       "style","slider",..
		       "Min",0,..
		       "Max",100,..
		       "Value",70,..
		       "units","pixels",..
		       "position"            , [xs yphi lslider h],...
		       "tag"                 , "Phi", ...
		       "callback"            , "update_phi()" ...
		       );
  Value_Phi = uicontrol("parent",fig,..
			"Position"  , [xv yphi lv h],...
			"Style"     , "text",...
			"String"    , "70",...
			"BackgroundColor",[1 1 1]);

  ypsi=yphi-(m+h);
  Psi_title = uicontrol( ...
      "parent"              , fig,...
      "style"               , "text",...
      "string"              , "Psi",...
      "units"               , "pixels",...
      "position"            , [xs ypsi+h lslider h],...
      "fontunits"           , "points",...
      "fontsize"            , 9,...
      "horizontalalignment" , "center", ...
      "background"          , [1 1 1], ...
      "tag"                 , "PsiTitle" ...
      );

  Slider_Psi=uicontrol("parent",fig,..
		       "style","slider",..
		       "Min",0,..
		       "Max",100,..
		       "Value",70,..
		       "units","pixels",..
		       "position"            , [xs ypsi lslider h],...
		       "tag"                 , "Psi", ...
		       "callback"            , "update_psi()" ...
		       );
    Value_Psi = uicontrol("parent",fig,..
			"Position"  , [xv ypsi lv h],...
			"Style"     , "text",...
			"String"    , "70",...
			"BackgroundColor",[1 1 1]);

    ybtn= ypsi -(m+h);  
    wb=40;eb=13;
    Start = uicontrol("parent",fig, ..
		       "Position"  , [xs ybtn wb h],...
		       "Style"     , "pushbutton",...
		       "String"    , "Start",...
		       "callback"  , "levitron_start()");
    Stop = uicontrol("parent",fig, ..
		       "Position"  , [xs+wb+eb ybtn wb h],...
		       "Style"     , "pushbutton",...
		       "String"    , "Stop",...
		       "callback"  , "levitron_stop()");
    Reinit = uicontrol("parent",fig, ..
		       "Position"  , [xs+2*(wb+eb) ybtn wb h],...
		       "Style"     , "pushbutton",...
		       "String"    , "Reinit",...
		       "callback"  , "levitron_reinit()");


    levitron_controls=[Slider_Height Value_Height
		       Slider_Theta Value_Theta
		       Slider_Phi Value_Phi
		       Slider_Psi Value_Psi]
    
    update_height(1.72)
    update_theta(0.28)
    update_phi(0)
    update_psi(0)
endfunction
//The callbacks
//-------------------------------------------------------------

function update_height(h)
  global levitron_controls
  Slider_Height=levitron_controls(1,1)
  Value_Height=levitron_controls(1,2)
  if argn(2)==1 then
    Slider_Height.Value=(h-1.6)*100/0.2;
  else
    h=Slider_Height.Value
    h=max(0.001,1.6+h*0.2/100)
  end
  Value_Height.String=msprintf("%.3f",h)
  update_state(3,h)
endfunction

function update_theta(t)
  global levitron_controls
  Slider_Theta=levitron_controls(2,1)
  Value_Theta=levitron_controls(2,2)
  if argn(2)==1 then
    Slider_Theta.Value=t*100;
  else
    t=Slider_Theta.Value
    t=t/100
  end
  Value_Theta.String=msprintf("%.3f",t)
  update_state(4,t)
endfunction

function update_phi(p)
  global levitron_controls
  Slider_Phi=levitron_controls(3,1)
  Value_Phi=levitron_controls(3,2)
  if argn(2)==1 then
    Slider_Phi.Value=p*100/360;
  else
    p=Slider_Phi.Value
    p=360*p/100
  end
  Value_Phi.String=msprintf("%.3f",p)
  update_state(5,p)
endfunction

function update_psi(p)
  global levitron_controls
  Slider_Psi=levitron_controls(4,1)
  Value_Psi=levitron_controls(4,2)
  if argn(2)==1 then
    Slider_Psi.Value=p*100/360;
  else
    p=Slider_Psi.Value
    p=360*p/100
  end
  Value_Psi.String=msprintf("%.3f",p)
  update_state(6,p)
endfunction


function levitron_start()
  global  Stop;Stop=%f;
  show();
endfunction

function levitron_stop()
  global Stop;Stop=%t
endfunction

function levitron_reinit()
  global y0 y1  Stop
  Stop=%t
  y1=y0
  update_height(1.72)
  update_theta(0.28)
  update_phi(0)
  update_psi(0)
endfunction


// Callbacks subsidiary functions
//-------------------------------
function update_state(k,value)
  global y1 state_changed init  Stop changed
  if ~changed then
    if or(k==(4:6)) then value=value*%pi/180;end
    y1(k)=value;
    if init then 
      set_levitron(H,y1),
    else 
      state_changed=%t;
    end
  end
endfunction

function set_levitron(H,q)
  O=H.user_data;
  Data=H.data;
  f=20 //Dilatation factor of X and Y coordinates
  XYZ=[matrix(Data.x-f*O(1),1,-1)
       matrix(Data.y-f*O(2),1,-1)
       matrix(Data.z-O(3),1,-1)];
  psi  =q(6); //precession:rotation Oxyz ->Ouvz
  theta=q(4);//nutation Ouvz -> Ouwz'
  phi  =q(5);// rotation propre Ouwz'->0x'y'z' (referentiel attache a la toupie)
       
  XYZ=euler(psi,theta,phi)*eulerm1(O(6),O(4),O(5))*XYZ
  
  drawlater()
  Data.x=f*q(1)+matrix(XYZ(1,:),4,-1);
  Data.y=f*q(2)+matrix(XYZ(2,:),4,-1);
  Data.z=q(3)+matrix(XYZ(3,:),4,-1);
  H.user_data(1:6)=q(1:6)';
  H.data=Data;
  drawnow()
endfunction

function show()
  global y1 state_changed init Stop changed
  //y1 is a copy of that can be updated by the gui (state_changed is true
  //if the GUI has modified its value).
  y=y1;
  init=%f;changed=%f
  dt=0.05
  if state_changed then y=y1;end
  [y,w,iw]=ode(y,0,dt,list(levitron_dyn,a,c,Mc)); y1=y;
  realtimeinit(dt)
  t0=dt
  k=1
  while %t
    if state_changed then 
      [y,w,iw]=ode(y1,t0,t0+dt,list(levitron_dyn,a,c,Mc));y1=y;
    else    
      [y,w,iw]=ode(y,t0,t0+dt,list(levitron_dyn,a,c,Mc),w,iw);y1=y;
    end
    if y(3)<=0 then Stop=%t,end
    if Stop then init=%t,break,end
    set_levitron(H,y(1:6)),
    if %t then
      changed=%t
      update_height(y(3))
      update_theta(y(4)*180/%pi)
      update_phi(modulo(y(5)*180/%pi,360))
      update_psi(modulo(y(6)*180/%pi,360))
      changed=%f
    end

    k=k+1;t0=t0+dt;
    realtime(k);
  end
endfunction
