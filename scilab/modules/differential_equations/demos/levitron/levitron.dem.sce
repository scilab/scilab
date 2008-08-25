//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

mode(-1)
exec SCI/modules/differential_equations/demos/levitron/levitron.sci;
exec SCI/modules/differential_equations/demos/levitron/levitron_gui.sci;
//Notations
//---------
//M:  The magnetic moment of the top
//Me: the net strength of the ring dipole
//R : the effective radius of the ring dipole (used as a normalization factor)
//A : transverse moment of inertia of the top
//C : polar  moment of inertia of the top
//m : the mass of the top
// x,y,z:    the coordinates of the center of mass of the top
//
// X=x/R, Y=y/R, Z=z/R, normalized coordinates
// a=A/(m*R^2)    normalized transverse moment of inertia of the top
// c=C/(m*R^2)    normalized polar  moment of inertia of the top
// Mc=(M*Me)/(4*%pi*m*g*R^4),   normalized magnetic moment of the top

//R=34.7d-3 
//m=0.02135
//A=1.32d-6
//C=2.20d-6
//http://www.koepken.de/levitron/maglev.java

a=0.089;
c=0.139;
Mc=8.2;  
//
%h=poly(0,'h');
Ph=(3*Mc*%h*(2*%h^2-3))^2-(1+%h^2)^7;
rPh=roots(Ph);
rPh=real(rPh(find(abs(imag(rPh))<0.00001& real(rPh)>0)));
mx=max(rPh);
mn=min(rPh);

h=1.74;//1.72;
Theta=0.005;
dTheta=0;
Psi=0;
dPsi=1.2;
Phi=0;
Omega_T=7.5;
dPhi=Omega_T-dPsi*cos(Theta);


q0=[0;    //X
    0;    //Y
    h;    //Z
    Theta;
    Phi;  
    Psi];  
p0=[0;    //X'
    0;    //Y'
    0;    //Z'
    a*dTheta; 
    dPsi*(a*sin(Theta)^2+c*cos(Theta)^2)+c*dPhi*cos(Theta)
    c*(dPhi+dPsi*cos(Theta))]

scf(100001);clf()
demo_viewCode("levitron.dem.sce");
H=build_levitron(h);
ax=gca();
y=[q0;p0];y0=y;y1=y;init=%t;state_changed=%f;
global y0 y1 state_changed init  Stop
set_levitron(H,y);
levitron_gui()

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
