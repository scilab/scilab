function [tt]=texprint(a)
// text = texprint(a) returns the Tex source code of the scilab variable a.
// a is a matrix (scalar, polynomial, rational) or a linear system
// (syslin list).
//!
//
// Copyright INRIA
typ=type(a)
tt=''

select typ
case 1 then  //scalars
  [m,n]=size(a)
  a=string(a)
  for l=1:m-1,tt=tt+strcat(a(l,:),'&')+'\cr ',end
  tt=strsubst(tt+strcat(a($,:),'&'),'%','')
  if  m*n<>1 then tt='\pmatrix{'+tt+'}',end
case 2 then //polynomials
  [m,n]=size(a);z=stripblanks(varn(a))
  for l=1:m
    for k=1:n,tt=tt+pol2tex(a(l,k))+'&',end
    tt=part(tt,1:length(tt)-1)+'\cr '
  end
  tt=part(tt,1:length(tt)-4);tt=strsubst(tt,'%','')
  if  m*n<>1 then tt='\pmatrix{'+tt+'}',end
case 4 then //booleans
  [m,n]=size(a)
  x='F'+emptystr(a);x(a)='T'
  for l=1:m-1,tt=tt+strcat(x(l,:),'&')+'\cr ';end
  tt=tt+strcat(x(m,:),'&')
  if  m*n<>1 then tt='\pmatrix{'+tt+'}',end
case 10 then //strings
  [m,n]=size(a)
  for l=1:m-1,tt=tt+strcat(a(l,:),'&')+'\cr ';end
  tt=tt+strcat(a(m,:),'&')
  if  m*n<>1 then tt='\pmatrix{'+tt+'}',end
case 8 then //int
  [m,n]=size(a)
  for l=1:m-1,tt=tt+strcat(a(l,:),'&')+'\cr ';end
  tt=tt+strcat(a(m,:),'&')
  if  m*n<>1 then tt='\pmatrix{'+tt+'}',end
case 5 then //sparse
  [ij,v,mn]=spget(a)
  ij=strsubst(string([mn,ij]),'.','')
  v=['Sparse matrix';string(v)]
  tt=strcat('('+ij(:,1)+'&'+ij(:,2)+'&) '+v,'\cr ')

case 6 then //boolean sparse
  [ij,v,mn]=spget(a)
  x='F'+emptystr(size(v,'*'),1);x(v)='T';v=x;
  ij=strsubst(string([mn;ij]),'.','')
  v=['Sparse matrix';string(v)]
  tt=strcat('('+ij(:,1)+'&'+ij(:,2)+'&) '+v,'\cr ')
case 16 then 
  a1=getfield(1,a)
  select a1(1)
  case 'r' then //rationals
    num=a.num;a=a.den
    [m,n]=size(a);z=stripblanks(varn(a))
    //
    for l=1:m
      for k=1:n,
	if degree(a(l,k))==0 then
	  num(l,k)=num(l,k)/coeff(a(l,k)),pol=1
	else
	  pol=0
	end
	nlk=pol2tex(num(l,k));
	if nlk=='0' then
	  tt=tt+'0&'
	else
	  if pol==1 then
	    tt=tt+nlk+'&'
	  else
	    dlk=pol2tex(a(l,k))
	    tt=tt+'{'+nlk+'}\over{'+dlk+'}&',
	  end
	end
      end
      tt=part(tt,1:length(tt)-1)+'\cr '
    end
    tt=part(tt,1:length(tt)-4)
    if m*n<>1 then tt='\pmatrix{'+tt+'}',end
  case 'lss' //linear state space
    if a(7)=='c' then der=' \dot{X}',else der=' \stackrel{+}{X}',end
    debut='\begin{eqnarray}';fin='\end{eqnarray}'
    debut='';fin=''
    tt=debut+der+' = '+texprint(a(2))+' x + '+...
	texprint(a(3))+'U \\ \\ Y = '+texprint(a(4))+' X '
    if norm(a(5),1)==0 then
      tt=tt+fin
    else
      tt=tt+' + '+texprint(a(5))+fin
    end
  else
    execstr('tt=%'+a1(1)+'_texprint(a)','errcatch')
  end
case 17  then
  a1=getfield(1,a)
  execstr('tt=%'+a1(1)+'_texprint(a)','errcatch')
else
  execstr('tt=%'+typeof(a)+'_texprint(a)','errcatch')
end
