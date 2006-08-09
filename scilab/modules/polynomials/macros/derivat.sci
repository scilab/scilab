function p=derivat(p)
//pd=derivat(p)  computes the derivative of the polynomial or rational
//function marix relative to the dummy variable
//!
// Copyright INRIA
t=type(p)
if t==1 then p=0*p,return,end
if t==2 then
  [m,n]=size(p);var=varn(p);
  for i=1:m
    for j=1:n
      pij=p(i,j);nij=degree(pij);
      if nij==0 then
	p(i,j)=0
      else
	pij=coeff(pij).*(0:nij),p(i,j)=poly(pij(2:nij+1),var,'c')
      end;
    end;
  end;
  return
end;

if t==16 then
  p1=p(1);
  if p1(1)=='r' then
    num=p(2);den=p(3)
    [m,n]=size(num)
    for i=1:m
      for j=1:n
	num(i,j)=derivat(num(i,j))*den(i,j)...
	    -num(i,j)*derivat(den(i,j))
	den(i,j)=den(i,j)**2
      end;
    end;
//    p=syslin(p(4),num,den)
    p(2)=num;p(3)=den;
    return
  end;
end;
error('incorrect data type')
endfunction
