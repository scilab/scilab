function []=Sgrayplot(x,y,z,strf,rect,aaint)
// Like fgrayplot but the function fec is used to smooth the 
// result 
// f is evaluated on the grid x.*.y 
// anf the result is plotted assuming that f is linear on the triangles 
// built on the grid 
// 
// ______ 
// | /| /|
// |/_|/_|  
// | /| /|
// |/_|/_|
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs<=0,s_mat=['t=-%pi:0.1:%pi;m=sin(t)''*cos(t);Sgrayplot(t,t,m);'];
         write(%io(2),s_mat);execstr(s_mat);
         return;end;
if rhs <= 3,strf="121";end
if rhs <= 4,rect=[-1,-1,1,1];end
if rhs <= 5,aaint=[10,2,10,2];end
p=prod(size(x));
q=prod(size(y));
noe_x = ones(y).*.x;
noe_y = y.*.ones(x);
z = matrix(z,p*q,1);
xxb=(1:p-1)';xx=[];
for i=0:q-2; xx=[ xx; xxb+p*i*ones(xxb)];end
[Ntr,vv]=size(xx);
trianl=[(1:Ntr)',xx,xx+ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];
trianl=[trianl;(Ntr+1:2*Ntr)',xx,xx+(p)*ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];
fec(noe_x',noe_y',trianl,z,strf," ",rect,aaint) 
// if you need to see the triangulation uncomment the following lines 
// 
//noeul=[noe_x',noe_y'];
//triang=size(trianl,1);
//i_meshvisu(3,rect);

function []=i_meshvisu(col,rect)
// Mesh visualisation 
// uses global variables 
[lhs,rhs]=argn(0);
if rhs<=0;col=1;end
if rhs<=1;rect=[mini(noeul(:,2)),mini(noeul(:,3)),maxi(noeul(:,2)),maxi(noeul(:,3))];end
if rhs<=2;iso='1';end
plot2d(1,1,[1],"031"," ",rect)
xset("clipgrf");
xx=trianl(:,2:4); xx=matrix(xx,prod(size(xx)),1);
x=noeul(xx,1)
x=matrix(x,triang,3);
y=noeul(xx,2)
y=matrix(y,triang,3);
x=[x,x(:,1)]';y=[y,y(:,1)]'
xpolys(x,y,col*ones(1,triang));
xset("clipoff");



