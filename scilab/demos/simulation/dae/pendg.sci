// Copyright INRIA

function [res,ires]=pendg(t,y,ydot)
x=y(1:3); u=y(4:6); lambda=y(7);
xp=ydot(1:3); up=ydot(4:6);
res(1:3)=xp-u;
res(4)=(M+m)*up(1)+m*l*cos(x(3))*up(3)-m*l*sin(x(3))*u(3)^2..
+lambda*fx(x(1),x(2))+k*u(1);
res(5)=(M+m)*up(2)+m*l*sin(x(3))*up(3)+m*l*cos(x(3))*u(3)^2..
+(M+m)*g+lambda*fy(x(1),x(2))+k*u(2);
res(6)=m*l*cos(x(3))*up(1)+m*l*sin(x(3))*up(2)+m*l^2*up(3)..
+m*g*sin(x(3));
res(7)=-(fx(x(1),x(2))*u(1)+fy(x(1),x(2))*u(2));
ires=0;
