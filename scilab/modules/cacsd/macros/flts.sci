function [y,xf]=flts(u,sl,x0)
// Copyright INRIA
[lhs,rhs]=argn(0)
if type(u)<>1 then error(53,1),end
if rhs<=1 then error(39),end
[nu,mu]=size(u)

if type(sl)<>16 then error(97,2),end
flag=sl(1);
select flag(1)
  case 'lss' then 
    if rhs==2 then x0=sl(6),end
    [nb,mb]=size(sl(3))
    if mb<>nu then
       error(60),
    end;
    select sl(7)
     case 'c' then error(94,2),
    end;
    np=maxi(degree(sl(5)))
    [xf,x]=ltitr(sl(2),sl(3),u(:,1:(mu-np)),x0)
   if type(sl(5))==1 then
      y=sl(4)*x+sl(5)*u
    else
      y=sl(4)*x+rtitr(sl(5),eye(sl(5)),u)
    end
   case 'r'  then  
    if lhs>1 then error('flts: invalid lhs'),end
    [num,den]=sl(['num','den']);[ns,ne]=size(num)
       select sl('dt')
        case 'c' then error(94,2),
       end;
    if ne<>nu then error(60), end;
    for l=1:1:ns,
       pp=den(l,1);
       for k=2:ne,[pg,uu]=bezout(pp,den(l,k)),pp=pp*uu(1,2),end
       nden(l)=pp
       for k=1:ne,nnum(l,k)=num(l,k)*pdiv(pp,den(l,k)),end,
    end;
    for l=1:ns, nm(l)=degree(nden(l))-maxi(degree(nnum(l,:))),end
    ly=mu+mini(nm)
    if rhs==3 then
       [mx,nx]=size(x0);maxdgd=maxi(degree(nden))
       if nx<maxdgd then
         error('AT LEAST '+string(maxdgd)+' PAST VALUES!')
       end;
       if mx<>ns+ne then error(60),end
    end;
      y=0*ones(ns,ly);
    for l=1:ns
      ddl=degree(nden(l))
      dnl=maxi(degree(nnum(l,:)))
      lent=ly-ddl+dnl
      select rhs
        case 2 then yl=rtitr(nnum(l,:),nden(l),u(:,1:lent));
                    [nn,mm]=size(yl);
                    y(l,1:mm)=yl;
//                    y=y(:,1:lent);
        case 3 then
               up=x0(1:ne,maxdgd-ddl+1:maxdgd);
               yp=x0(ne+l,maxdgd-ddl+1:maxdgd);
               y(l,:)=rtitr(nnum(l,:),nden(l),u(:,1:lent),up,yp);
      end;
    end,
    l=size(y,2);
    y=y(:,1:min(mu,l));
  else error(97,2)
end;
endfunction
