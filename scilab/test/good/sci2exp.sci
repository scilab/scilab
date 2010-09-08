// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function t=sci2exp(a,nom,lmax)
// sci2exp - convert a variable to an expression
//%SYNTAX
// t=sci2exp(a [,nam] [,lmax])
//%PARAMETERS
//   a  : matrix of scalar or polynomials
//   nam: character string
//   t  : vector of string, contains the expression definition
//   lmax : maximum line lengh (0 for no line length control)
//%Example
//  a=[1 2;3 4]
//  sci2exp(a,'aa')
//!

  deff('x=String(a)',['x=string(a)'
		      'x=strsubst(x,''Nan'',''%nan'')'
		      'x=strsubst(x,''Inf'',''%inf'')'
		     ])

  
[lhs,rhs]=argn(0)
$ // import global variable $ in sci2exp context for efficiency
select rhs
case 1 then 
  named=%f
  lmax=90
case 2 then
  if type(nom)==1 then
    lmax=nom
    named=%f
  else
    named=%t
    lmax=90
  end
case 3 then
  named=%t
  if type(nom)==1 then
    [lmax,nom]=(nom,lmax)
  end
end
dots='..';
select type(a)
case 1 then
  t=mat2exp(a,lmax)
case 2 then
  t=pol2exp(a,lmax)
case 4 then
  t=log2exp(a,lmax)
case 5 then
  t=sp2exp(a,lmax)
case 6 then
  t=sp2exp(a,lmax)
case 7 then
  t=sp2exp(a,lmax)
case 8 then 
  t=int2exp(a,lmax)
case 10 then
  t=str2exp(a,lmax)
case 13 then
  if named then
    t=fun2string(a,nom)
  else
    t=fun2string(a,'%fun')
  end
  t(1)=part(t(1),10:length(t(1)))
  t($)=[]
  t=sci2exp(t,lmax)
  t(1)='createfun('+t(1)
  t($)=t($)+')'
case 15 then
  t=list2exp(a,lmax)
case 16 then
  t=tlist2exp(a,lmax)
case 17 then
  t=mlist2exp(a,lmax)
case 11 then
  t=func2exp(a,lmax)
  named=%f
case 129 then
  t=imp2exp(a,lmax)
else
//  execstr('t='+typeof(a)+'2exp(a,lmax)')
   error(msprintf(gettext("%s: This feature has not been implemented: Variable translation of type %s.\n"),"sci2exp",string(type(a))));
end,
if named&and(type(a)<>[11 13]) then
  t(1)=nom+' = '+t(1)
end
endfunction

function t=str2exp(a,lmax) 
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  [m,n]=size(a),
  dots='.'+'.'
  t=[];
  quote=''''

  a=strsubst(a,quote,quote+quote)
  dquote='""'
  a=strsubst(a,dquote,dquote+dquote)
  a=quote(ones(a))+a+quote(ones(a))

  for i=1:m
    x=emptystr();
    for j=1:n,
      y=a(i,j);
      y=dquote+part(y,2:length(y))
      y=part(y,1:length(y)-1)+dquote
      if y=='''''' then y='emptystr()',end
      if lmax==0|length(x($))+length(y)<lmax then
	if j==1 then
	  x=y
	else
	  x($)=x($)+','+y,
	end
      else
	if j>1 then 
	  x($)=x($)+','+dots;
	  x($+1)=y
	else
	  x=y
	end
      end
    end
    if i<m then x($)=x($)+';',end
    if lmax>0 then
      t=[t;x]
    else
      t=t+x
    end
  end,
  if lmax>0&sum(length(t))<lmax then
    t=strcat(t)
  end
  if m*n>1 then
    t(1)='['+t(1)
    t($)=t($)+']'
  end
endfunction

function t=mat2exp(a,lmax)
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  if size(a,'*')==0 then t='[]',return,end
  [m,n]=size(a);
  if m<0 then
    t=mat2exp(a+0);
    if t=='1' then
      t='eye()';
    else
      t='('+t+')*eye()';
    end
    return
  end
  a=String(a);
  dots='.'+'.'
  t=[];
  if n==1 then
    x=strcat(a,';')
    lx=length(x)
    if lmax==0|lx<lmax then
      t=x;
    else
      ind=strindex(x,';');
      k1=1;l=0;I=[];
      while %t
	if lx-l<lmax|k1>length(ind) then,break,end
	k2=k1-1+max(find(ind(k1:$)<l+lmax))
	I=[I ind(k2)];
//	t=[t;part(x,l+1:ind(k2))]
	k1=k2+1
	l=ind(k2)
      end
      t=[t;strsplit(x,I)];
    end
  elseif m==1 then      
    x=strcat(a,',')
    lx=length(x)
    if lmax==0|lx<lmax then
      t=x;
    else
      ind=strindex(x,',');
      k1=1;l=0;I=[];
      while %t
	if lx-l<lmax|k1>length(ind) then break,end
	k2=k1-1+max(find(ind(k1:$)<l+lmax))
	I=[I ind(k2)];
//	t=[t;part(x,l+1:ind(k2))+dots]
	k1=k2+1
	l=ind(k2)
      end
      x=strsplit(x,I);x(1:$-1)=x(1:$-1)+dots;
      t=[t;x]
    end
  else
    for i=1:m
      x=strcat(a(i,:),',')
      if i<m then x=x+';',end
      lx=length(x)
      if lmax==0 then
	t=t+x
      elseif lx<lmax then
	t=[t;x]
      else
	ind=strindex(x,',');
	k1=1;l=0;I=[];
	while %t
	  if lx-l<lmax|k1>length(ind) then break,end
	  k2=k1-1+max(find(ind(k1:$)<l+lmax))
	  I=[I ind(k2)];
//	  t=[t;part(x,l+1:ind(k2))+dots]
	  k1=k2+1
	  l=ind(k2)
	end
	x=strsplit(x,I);x(1:$-1)=x(1:$-1)+dots;x(2:$)=' '+x(2:$);
	t=[t;x]
      end
    end
  end
  if m*n>1 then
    t(1)='['+t(1)
    t($)=t($)+']'
  end
endfunction

function t=pol2exp(a,lmax)
  $
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end

  [m,n]=size(a),var=' ';lvar=1
  var=varn(a),lvar=length(var);
  while part(var,lvar)==' ' then lvar=lvar-1,end
  var=part(var,1:lvar);
  if m<0 then
    t=pol2exp(a+0)
    t='('+t+')*eye()'
    return
  end
  t=[];
  for i=1:m
    x=emptystr(1)
    for j=1:n,
      v=a(i,j);d=degree(v);
      v=coeff(v);
      k0=1;while (k0<d+1)&(v(k0))==0 then k0=k0+1,end
      y=emptystr(1)
      nul=%t
      for k=k0:(d+1),
	s=String(v(k))
	if s<>'0' then
	  nul=%f
	  if part(s,1)==' ' then s=part(s,2:length(s)),end
	  mnm=emptystr(1)
	  if k>0 then 
	    mnm=var
	    if k>2 then 
	      mnm=mnm+'^'+string(k-1),
	    end
	  end
	  if k>1 then
	    if string(imag(v(k)))<>'0' then s='('+s+')',end
	    if s=='1' then 
	      s=mnm
	    elseif s=='-1' then
	      s='-'+mnm
	    else
	      s=s+'*'+mnm
	    end
	    if k>k0&part(s,1)<>'-' then s='+'+s,end
	  end
	  if lmax==0|length(y($))+length(s) <lmax then
	    y($)=y($)+s
	  else
	    y($)=y($)+dots
	    y($+1)=s
	  end
	end
      end
      if nul then y='0*'+var,end
      [ny,my]=size(y)

      if lmax==0|length(x($))+length(y(1))<lmax then
	if j==1 then
	  x=y(1)
	else
	  x($)=x($)+','+y(1),
	end
	if ny>1 then x($+1:$+ny-1)=y(2:ny),end
      else
	if length(x($))==0 then
	  x($:$+ny-1) = y;
	else
	  x($)=x($)+','+dots;
	  x($+1:$+ny)=y
	end
      end
    end
    if i<m then x($)=x($)+';',end
    if lmax>0 then
      t=[t;x]
    else
      t=t+x
    end
  end,
  if lmax>0&sum(length(t))<lmax then
    t=strcat(t)
  end
  if m*n>1 then
    t(1)='['+t(1)
    t($)=t($)+']'
  end
endfunction

function t=list2exp(l,lmax)
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  dots='.'+'.';
  t='list('
  n=length(l)
  for k=1:n
    lk=l(k)
    sep=',',if k==1 then sep=emptystr(),end
    if type(lk)==15 then
      t1=list2exp(lk,lmax)
    elseif type(lk)==16 then 
      t1=tlist2exp(lk,lmax)
    elseif type(lk)==17 then 
      t1=mlist2exp(lk,lmax) 
    else
      t1=sci2exp(lk,lmax)
    end
    if size(t1,'*')==1&(lmax==0|max(length(t1))+length(t($))<lmax) then
      t($)=t($)+sep+t1
    else
      t($)=t($)+sep+dots
      t=[t;t1]
    end
    lk=null()
  end
  t($)=t($)+')'
endfunction

function t=tlist2exp(l,lmax)
  $;
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  dots='.'+'.';
  t='tlist('
  n=length(l)
  for k=1:n
    lk=l(k)
    sep=',',if k==1 then sep=emptystr(),end
    if type(lk)==15 then
      t1=list2exp(lk,lmax)
    elseif type(lk)==16 then
      t1=tlist2exp(lk,lmax)
    elseif type(lk)==17 then
      t1=mlist2exp(lk,lmax) 
    else
      t1=sci2exp(lk,lmax)
    end
    if size(t1,'*')==1&(lmax==0|max(length(t1))+length(t($))<lmax) then
      t($)=t($)+sep+t1
    else
      t($)=t($)+sep+dots
      t=[t;t1]
    end
  end
  t($)=t($)+')'

endfunction

function t=mlist2exp(l,lmax)
  $;
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  dots='.'+'.';
  t='mlist('
  n=size(definedfields(l),'*')
  for k=1:n
    lk=getfield(k,l)
    sep=',',if k==1 then sep=emptystr(),end
    if type(lk)==15 then
      t1=list2exp(lk,lmax)
    elseif type(lk)==16 then
      t1=tlist2exp(lk,lmax)
    elseif type(lk)==17 then
      t1=mlist2exp(lk,lmax)  
    else
      t1=sci2exp(lk,lmax)
    end
    if size(t1,'*')==1&(lmax==0|max(length(t1))+length(t($))<lmax) then
      t($)=t($)+sep+t1
    else
      t($)=t($)+sep+dots
      t=[t;t1]
    end
  end
  t($)=t($)+')'

endfunction

function t=log2exp(a,lmax)
  $;
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  [m,n]=size(a),
  a1=matrix(a,m*n,1)
  F='%f'
  T='%t'
  a=F(ones(m*n,1))
  k=find(a1);
  if k<>[] then
    a(k)=T(ones(size(k,'*'),1));
  end
  a=matrix(a,m,n);
  dots='.'+'.'
  t=[];
  for i=1:m
    x=emptystr()
    for j=1:n,
      y=a(i,j);
      if lmax==0|length(x($))+length(y)<lmax then
	if j==1 then
	  x=y
	else
	  x($)=x($)+','+y,
	end
      else
	x($)=x($)+','+dots;
	x($+1)=y
      end
    end
    t=[t;x]
  end,
  if lmax==0|lmax>0&sum(length(t))<lmax then
    t=strcat(t,';')
  end
  if m*n>1 then
    t(1)='['+t(1)
    t($)=t($)+']'
  end

endfunction

function t=func2exp(a,lmax)
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  [out,in,text]=string(a)
  if out==[] then out=emptystr(),end
  text=str2exp(text,lmax)
  nt=size(text,'*')
  if named then mac=nom,else mac='mac',end
  if nt==1 then
    t='deff(''['+strcat(out,',')+']='+mac+'('+strcat(in,',')+')'','+text+')'
  else
    text($)=text($)+')'
    semi=';'
    text(1:$-1)=text(1:$-1)+semi(ones(nt-1,1))'
    t=['deff(''['+strcat(out,',')+']='+mac+'('+strcat(in,',')+')'','+text(1)
       text(2:$)]
  end
endfunction

function t=imp2exp(a,lmax)
  [lhs,rhs]=argn(0)
  if rhs<2 then lmax=0,end
  v=[a(1),a(2),a(3)]
  if a(2)==1 then
    t=sci2exp(a(1))+':'+sci2exp(a(3))
  else
    t=sci2exp(a(1))+':'+sci2exp(a(2))+':'+sci2exp(a(3))
  end
endfunction

function t=sp2exp(a,lmax)
  [lhs,rhs]=argn(0)
  dots='..';
  if rhs<2 then lmax=0,end
  [ij,v,mn]=spget(a)
  t=sci2exp(ij,lmax)
  v=sci2exp(v,lmax)
  mn=sci2exp(mn,lmax)

  t(1)='sparse('+t(1)
  if lmax==0|length(t($))+length(v(1))+1<lmax then
    t($)=t($)+','+v(1)
    t=[t;v(2:$)]
  else
    t($)=t($)+','+dots
    t=[t;v]
  end
  if lmax==0|length(t($))+length(mn(1))+1<lmax then
    t($)=t($)+','+mn(1)
    t=[t;mn(2:$)]
  else
    t($)=t($)+','+dots
    t=[t;mn]
  end
  t($)=t($)+')'
endfunction


function t=int2exp(a,lmax)
  it=inttype(a)
  if it>10 then f='uint',else f='int',end
  f=f+string(8*modulo(it,10))
  t=mat2exp(double(a),lmax)
  t(1)=f+'('+t(1)
  t($)=t($)+')'
endfunction
