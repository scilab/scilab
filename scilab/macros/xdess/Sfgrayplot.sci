function []=Sfgrayplot(x,y,f,strf,rect,aaint)
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
if rhs<=0,s_mat=['deff(''[z]=surf(x,y)'',''z=x**3+y'');';
                'Sfgrayplot(-1:0.1:1,-1:0.1:1,surf);'];
         write(%io(2),s_mat);execstr(s_mat);
         return;end;
if rhs <= 3,strf="121";end
if rhs <= 4,rect=[-1,-1,1,1];end
if rhs <= 5,aaint=[10,2,10,2];end
if type(f)==11 then comp(f),end;
p=prod(size(x));
q=prod(size(y));
noe_x = ones(y).*.x;
noe_y = y.*.ones(x);
fun = feval(x,y,f);
fun = matrix(fun,p*q,1);
xxb=(1:p-1)';xx=[];
for i=0:q-2; xx=[ xx; xxb+p*i*ones(xxb)];end
[Ntr,vv]=size(xx);
trianl=[(1:Ntr)',xx,xx+ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];
trianl=[trianl;(Ntr+1:2*Ntr)',xx,xx+(p)*ones(xx),xx+(p+1)*ones(xx),0*ones(xx)];
fec(noe_x',noe_y',trianl,fun,strf," ",rect,aaint) 

