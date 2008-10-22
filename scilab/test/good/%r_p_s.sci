function f=%r_p_s(f,s)
// %r_p_s(p,n) computes f^s for f a rational matrix s scalar or vector
//!
// Copyright INRIA
if s==[] then f=[],return,end
if or(imag(s)<>0)|or(int(s)<>s) then error(30),end

[m,n]=size(f('num'))
[ms,ns]=size(s)
if ms==1&ns==1 then
  if m==1|n==1 then //Element wise exponentiation f.^s with f vector
    if s<0 then 
      num=f('num')
      if or(abs(coeff(num(:)))*ones(maxi(degree(num))+1,1)==0) then
	error(27)
      end
      s=-s
      f=rlist(f('den').^s,num.^s,f('dt'))
    else
      f=rlist(f('num').^s,f('den').^s,f('dt'))
    end
  elseif m==n then //square matrix exponentiation f^s
    if s==0 then f=eye(m,n),return,end
    if s<0 then f=invr(f),s=-s,end
    f1=f;for k=2:s,f=f*f1;end
  else
    error(20,1)
  end
elseif ms==1|ns==1 then // Element wise exponentiation f.^s with f "scalar"
  if m<>1|n<>1 then error(43),end
  kp=find(s>=0)
  kn=find(s<0)
  num=ones(s)
  den=ones(s)
  num(kp)=f('num').^s(kp)
  den(kp)=f('den').^s(kp)
  if abs(coeff(f('num')))*ones(degree(f('num'))+1,1)==0 then
    error(27)
  end
  num(kn)=f('den').^(-s(kn))
  den(kn)=f('num').^(-s(kn))
  f=rlist(num,den,[])
else
  error(30)
end
    
endfunction
