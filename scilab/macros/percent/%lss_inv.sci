function x=%lss_inv(a)
// Copyright INRIA
d=a(5);
[m,n]=size(d);
polyn=(type(d)==2);constant=(type(d)==1);
if constant&(m==n) then 
  minsv=mini(svd(d));rcd=rcond(d);s=poly(0,'s');
end
if constant&(m<>n) then 
  minsv=mini(svd(d));s=poly(0,'s');
end

if polyn then rcd=0;minsv=0;s=poly(0,varn(d));end
if m==n then 
  if rcd > 1.d-6 then
    x=invsyslin(a)
  else
    h=systmat(a);
    se=rand('seed');
    valfa=rand(1,10,'normal')/100;
    rand('seed',se);
    www=[];
    for k=1:10
    www=[www,rcond(horner(h,valfa(k)))];end
    [w,k1]=maxi(www);alfa=valfa(k1);
    x=invrs(a,alfa);
  end
elseif m<n then
  warning('non square system! --> right inverse')
  if minsv > 1.d-6 then
    x=invsyslin(a)
  else
    [stmp,ws]=rowregul(a,0,0);
    if mini(svd(stmp(5))) > 1.d-6 then
      x=invsyslin(stmp)*ws
    else
      error(19)
    end
  end
elseif m>n then
  warning('non square system! --> left inverse')
  if minsv > 1.d-6 then
    x=invsyslin(a)
  else
    [stmp,ws]=rowregul(a,0,0);
    if mini(svd(stmp(5))) > 1.d-6 then
      x=invsyslin(stmp)*ws
    else
      error(19)
    end
  end
end
endfunction
