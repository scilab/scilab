//Copyright INRIA

//   Testing dassl/dasrt for E xdot - A x = 0  (index one)
//   x(0)=x0;   xdot(0)=xd0
Eps=1.e-9
rand('seed',0);
nx=5;
E=rand(nx,1)*rand(1,nx);A=rand(nx,nx);
//         Index 1
[Si,Pi,Di,o]=penlaur(E,A);pp=Si*E;[q,m]=fullrf(pp);
x0=q(:,1);x0d=pinv(E)*A*x0;
deff('[r,ires]=g(t,x,xdot)','r=E*xdot-A*x;ires=0');
t=[1,2,3];t0=0;info=list([],0,[],[],[],0,0);
x=dassl([x0,x0d],t0,t,g,info);x1=x(2:nx+1,:);
if norm(pp*x1-x1,1)  > Eps then pause,end
//x(4) goes through 1 at  t=1.5409711:
t=1.5409711;ww=dassl([x0,x0d],t0,t,g,info);
if abs(ww(5)-1)  > 1000*Eps then pause,end
//Find this fact with dasrt:
deff('[rt]=surface(t,y,yd)','rt=y(4)-1');nbsurf=1;
[yyy,nnn]=dasrt([x0,x0d],t0,t,g,nbsurf,surface,info);
//Defining Jacobian 
deff('pd=j(t,y,ydot,cj)','pd=cj*E-A');
x=dassl([x0,x0d],t0,t,g,j,info);x2=x(2:nx+1,1);
if norm(x2-ww(2:nx+1,1),1)  > Eps then pause,end
[yyy1,nnn]=dasrt([x0,x0d],t0,t,g,j,nbsurf,surface,info);
//Assuming x0d is not known:
x0d=ones(x0);info(7)=1;
x=dassl([x0,x0d],t0,t,g,info);
xn=dassl([x0,x0d],t0,t,g,j,info);
if norm(x-xn,1)  > Eps then pause,end

