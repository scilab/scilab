function [y,x]=csim(u,dt,sl,x0,tol)
//Syntax:
//  [y [,x]]=csim(u,dt,sl,[x0]) 
// simulation of the controlled linear system sl.
// sl is assumed to be a continuous-time system.
// u is the control and x0 the initial state.
//
//u can be:
// - a function 
//    [inputs]=u(t)
// - a list
//    list(ut,parameter1,....,parametern) such that
//    inputs=ut(t,parameter1,....,parametern)
// - the character string 'impuls' for impulse response calculation
//    (here sl is assumed SISO without direct feedthrough and x0=0)
// - the character string 'step' for step response calculation 
//    (here sl is assumed SISO without direct feedthrough and x0=0)
//dt is a vector of instants with dt(1) = initial time
//                   that is:           x0=x
//                                          dt(1)
//
//y matrix such that:
//  y=[y       y  ...  y     ]
//      dt(1)   dt(2)   dt(n)
//x matrix such that:
//  x=[x       x  ...  x     ]
//      dt(1)   dt(2)   dt(n)
//
//See also:
// dsimul flts ltitr rtitr ode impl
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
//
if rhs<3 then error(39),end

if type(sl)<>16 then error(56,1),end
flag=sl(1)
select flag(1)
  case 'lss' then ,
  case  'r'  then sl=tf2ss(sl)
  else  error(97,1),
end;
if sl(7)<>'c' then warning('csim: time domain is assumed continuous'),end
//
[a,b,c,d]=sl(2:5);
if type(d)==2&degree(d)>0 then d=coeff(d,0);warning('D set to constant');end
[ma,mb]=size(b);
//
imp=0;text='if t==0 then y=0, else y=1,end'
//
select type(u)
 case 10 then //
    if mb<>1 then error(95,1);end;
    if part(u,1)=='i' then
           imp=1;
           if norm(d,1)<>0 then
               warning('direct feedthrough (d) <> 0;set to zero');
               d=0*d;
           end;
    end;
    deff('[y]=u(t)',text);
 case 11,comp(u)
 case 13,
 case 1 then
   [mbu,ntu]=size(u);
   if mbu<>mb | ntu<>size(dt,'*') then error('wrong size of u'), end
 case 15 then
    uu=u(1),
    if type(uu)==11 then 
      comp(uu),
      u(1)=uu,
    end
 else error(44,2)
end;
//
if rhs==3 then x0=sl(6),end
if imp==1 then x0=0*x0,end
nt=size(dt,'*');x=0*ones(ma,nt)
[a,v]=balanc(a);
//apply transformation u without matrix inversion
[k,l]=find(v<>0) //get the permutation
//apply right transformation 
v=v(k,l);c=c(:,k)*v; 
//apply left transformation 
v=diag(1 ./diag(v));b=v*b(k,:);x0=v*x0(k)

[a,v,bs]=bdiag(a,1);b=v\b;c=c*v;x0=v\x0;
//
if type(u)==1 then
   ut=u;
   if min(size(ut))==1 then ut=matrix(ut,1,-1),end
   deff('[y]=u(t)',['ind=find(dt<=t);nn=ind($)'
		    'if (t==dt(nn)|nn==nt) then '
		    '   y=ut(:,nn)'
		    'else '
		    '   y=ut(:,nn)+(t-dt(nn))/(dt(nn+1)-dt(nn))*(ut(:,nn+1)-ut(:,nn))'
		    'end']);
   deff('[ydot]=%sim2(%tt,%y)','ydot=ak*%y+bk*u(%tt)');
elseif type(u)<>15 then
  deff('[ydot]=%sim2(%tt,%y)','ydot=ak*%y+bk*u(%tt)');
  ut=ones(mb,nt);for k=1:nt, ut(:,k)=u(dt(k)),end
else
  %sim2=u
  tx=' ';for l=2:size(u), tx=tx+',%'+string(l-1);end;
  deff('[ydot]=sk(%tt,%y,u'+tx+')','ydot=ak*%y+bk*u(%tt'+tx+')');
  %sim2(0)=sk;u=u(1)
  deff('[ut]=uu(t)',...
     ['['+part(tx,3:length(tx))+']=%sim2(3:'+string(size(%sim2))+')';
      'ut=ones(mb,nt);for k=1:nt, ut(:,k)=u(t(k)'+tx+'),end'])
ut=uu(dt);
end;
//simulation
k=1;
for n=bs',
  kk=k:k+n-1
  ak=a(kk,kk)
  bk=b(kk,:)
  nrmu=maxi([norm(bk*ut,1),norm(x0(kk))])
  if nrmu > 0 then
    if rhs<5 then
      atol=1.d-10*nrmu;rtol=atol/100
    else
      atol=tol(1);rtol=tol(2)
    end
    x(kk,:)=ode('adams',x0(kk),dt(1),dt,rtol,atol,%sim2)
    if imp==1 then x(kk,:)=ak*x(kk,:)+bk*ut,end
  end;
  k=k+n
end;
if imp==0 then y=c*x+d*ut,else y=c*x,end
if lhs==2 then x=v*x,end
endfunction
