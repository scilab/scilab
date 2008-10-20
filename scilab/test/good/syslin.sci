function [sl]=syslin(domain,a,b,c,d,x0)
// Copyright INRIA
[lhs,rhs]=argn(0)
//
// check domain 
select type(domain)
case 1 then  //sampled system
  if size(domain,'*')<=2 then
    tp=domain
  else
    error('domain (1rst argument of syslin) must be a scalar')
  end
  z='z'
case 10 //continuous or discrete
  if size(domain,'*')<>1 then
    error('domain (1rst argument of syslin) must be a single string')
  end
  domain=part(domain,1)
  select domain
  case 'c' then 
    z='s'
  case 'd' then 
    z='z'
  else 
    error(domain+' : unknown time domain')
  end;
else 
  error('1rst argument of syslin should be a string, a scalar or a [] matrix')
end;
//============================================================================
if rhs==2 then //syslin(domaine,sys)

  if type(a)<>16 then 
    error()
  else
    if a(1)(1)=='r'|a(1)(1)=='lss' then
      sl=a;
      sl('dt')=domain
    else
      error('syslin : H must be a linear state space or a transfer function')
    end
  end
//============================================================================
elseif rhs==3 then // syslin(domaine,num,den)
  num=a;den=b
  if type(num)>2 | type(den)>2 then
    error('syslin : N and D must be matrix of numbers or of polynomials')
  end
  if or(size(num)<>size(den)) then
    error('syslin : N and D have inconsistent dimensions')
  end

  if type(num)==2 & type(den)==2 then
    if varn(num)<>varn(den) then 
      error('syslin : N and D have inconsistent formal variable names')
    end
  end
  if type(num)==1 then
    num=num*poly(1,z,'c')
  end
   if type(den)==1 then
    den=den*poly(1,z,'c')
  end
  
  sl=rlist(varn(num,z),varn(den,z),domain)
//============================================================================
elseif rhs>3 then // syslin(domaine,A,B,C [,D [X0]])
  if type(a)<>1 then
    error('syslin : A must be a square matrix of numbers')
  end
  [ma,na]=size(a);
  if ma<>na then 
    error('syslin : A must be a square matrix of numbers')
  end
  if type(b)<>1 then
    error('syslin : B must be a  matrix of numbers')
  end
  [mb,nb]=size(b);
  if na<>mb&mb<>0 then 
    error('syslin : row dimension of B do not agree dimensions of A')
  end
  if type(c)<>1 then
    error('syslin : C must be a  matrix of numbers')
  end
  [mc,nc]=size(c);
  if na<>nc&nc<>0 then 
    error('syslin : column dimension of C do not agree dimensions of A')
  end
  if rhs<6 then
    x0=0*ones(na,1)
  else
    if type(x0)>1 then
      error('syslin : X0 must be a vector of numbers')
    end
    [mx,nx]=size(x0);
    if mx<>na|nx<>min(na,1) then 
      error('syslin : dimensions of X0 do not agree')
    end
  end
  if rhs<5  then
    d=0*ones(mc,nb)
  else
    if type(d)>2 then
      error('syslin : D must be a  matrix of numbers or polynomials')
    end
    [md,nd]=size(d);
    if c*b<>[] then
      if mc<>md|nb<>nd then 
        error('syslin : column dimension of D do not agree dimensions of B or C')
      end
    end
  end
  sl=lsslist(a,b,c,d,x0,domain)
end
 
endfunction
