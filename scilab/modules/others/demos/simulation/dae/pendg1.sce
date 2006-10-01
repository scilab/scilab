// Copyright INRIA

//
// Sliding pendulum, curve #1
//

mode(1)

getf("SCI/demos/simulation/dae/pendg.sci")
getf("SCI/demos/simulation/dae/pendc1.sci")

g=10; l=1; m=1; M=1; k=0.5;
x0=[1;1;0]; u0=[0;0;0];
y0=[x0;u0;0];
ud0=[0;-g;0]; yd0=[u0;ud0;0];

t0=0; T=t0:0.05:20;

info=list([],0,[],[],[],0,0);
atol=[0.0001;0.0001;0.0001;0.0001;0.0001;0.0001;0.001];
rtol=atol;

sol=dassl([y0,yd0],t0,T,rtol,atol,pendg,info);

H=build_sliding_pendulum ()
draw_sliding_pendulum(H,y0(1:3))

//visualization of the result
realtimeinit(0.05);realtime(0) 
for i=1:size(sol,2)
  realtime(i)
  draw_sliding_pendulum(H,sol(2:4,i))
end
xdel()

