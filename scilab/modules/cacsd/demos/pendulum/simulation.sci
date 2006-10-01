

function [xdot]=ivpd(t,x)
//ydot=ivpd(t,y) non linear equations of the pendulum
// y=[x;d(x)/dt,teta,d(teta)/dt].
// mb, mc, l must be predefined 
//!
g=9.81;
u=0
qm=mb/(mb+mc)
cx3=cos(x(3))
sx3=sin(x(3))
d=4/3-qm*cx3*cx3
xd4=(-sx3*cx3*qm*x(4)**2+2/(mb*l)*(sx3*mb*g-qm*cx3*u))/d
//
xdot=[x(2);
      (u+mb*(l/2)*(sx3*x(4)**2-cx3*xd4))/(mb+mc);
      x(4);
      xd4]



function [y,xdot]=pendu(x,u)
//[y,xdot]=pendu(x,u) input (u) - output (y) function of the pendulum
//!
g=9.81;
qm=mb/(mb+mc)
cx3=cos(x(3))
sx3=sin(x(3))
d=4/3-qm*cx3*cx3
xd4=(-sx3*cx3*qm*x(4)**2+2/(mb*l)*(sx3*mb*g-qm*cx3*u))/d
//
xdot=[x(2);
      (u+mb*(l/2)*(sx3*x(4)**2-cx3*xd4))/(mb+mc);
      x(4);
      xd4]
//
y=[x(1);
   x(3)]



function [xdot]=regu(t,x)
//xdot=regu(t,x) simulation of the compound system
// pendulum - observer - compensator
//!
xp=x(1:4);xo=x(5:8);
u =-kr*xo   //control
[y,xpd]=pendu(xp,u)
xod=(f-k*h)*xo+k*y+g*u
xdot=[xpd;xod]



