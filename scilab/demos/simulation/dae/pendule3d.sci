// Copyright INRIA

function c=gg(x)
c=x'*x-l*l;

function c=ggp(x,u)
c=2*x'*u;

function c=ggpp(x,u,lambda)
c=2*(u'*u+x'*(-2*lambda/m*x-[0;0;g]));

function [res,ires]=index2(t,y,ydot)
x=y(1:3); u=y(4:6); lambda=y(7);
xp=ydot(1:3); up=ydot(4:6); lambdap=ydot(7);
res(1:3)=xp-u;
res(4:6)=up+2*lambda/m*x+[0;0;g];
res(7)=ggp(x,u);
ires=0;

function [res,ires]=index1(t,y,ydot)
x=y(1:3); u=y(4:6); lambda=y(7);
xp=ydot(1:3); up=ydot(4:6); lambdap=ydot(7);
res(1:3)=xp-u;
res(4:6)=up+2*lambda/m*x+[0;0;g];
res(7)=ggpp(x,u,lambda);
ires=0;

function [res,ires]=index1s(t,y,ydot)
x=y(1:3); u=y(4:6); lambda=y(7);
xp=ydot(1:3); up=ydot(4:6); lambdap=ydot(7);
res(1:3)=xp-u;
res(4:6)=up+2*lambda/m*x+[0;0;g];
res(7)=Beta*Beta*gg(x)+2*alfa*ggp(x,u)+ggpp(x,u,lambda);
ires=0;
