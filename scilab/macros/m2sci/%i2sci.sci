function [stk,txt,top]=%i2sci()
//
//!
// Copyright INRIA
txt=[]
rhs=abs(evstr(op(3)))-2
sto=stk(top);top=top-1
sfrom=stk(top);top=top-1
top=top-rhs+1
s2=stk(top)
if rhs==1 then

  if s2(5)=='10'&sto(5)=='?'  then // a.x=b with a undefined
    txt=sto(1)+' = struct()';sto(5)='16';
  end
  if type(s2(1))==1 then // recursive insertion
    [stk,t1,top]=%i2sci_rec()
    txt=[txt;t1];
  elseif sto(5)=='10'|sfrom(5)=='10' then //insertion of strings
    txt=[txt;sto(1)+' = mtlb_is'+rhsargs([sto(1),sfrom(1),s2(1)])]
    stk=list(op(2),'-1','?','?',sto(5))
  else
    if s2(5)=='10' then //x('i')=y
      txt=[txt;sto(1)+'('+s2(1)+') = '+sfrom(1)]
      stk=list(op(2),'-1','?','1','?') 
    elseif s2(1)<>':' then // x(i)=y
      if sto(3)=='0'|sto(4)=='0' then // insertion in an empty matrix
	txt=[txt;sto(1)+'(1,'+s2(1)+') = '+sfrom(1)+';']
	stk=list(op(2),'-1','?','?',sto(5))
      else
	if (sto(3)=='1'&sto(4)=='1') then //insertion in a scalar
	  [stk,t1,top]=%i2sci_s();txt=[txt;t1];
	elseif (sto(3)=='1'&sto(4)<>'1') then //insertion in a row vector
	  [stk,t1,top]=%i2sci_r();txt=[txt;t1];
	elseif (sto(3)<>'1'&sto(4)=='1') //insertion in a column vector
	  [stk,t1,top]=%i2sci_c();txt=[txt;t1];
	else // insertion in a general matrix or unknown sizes
	  [stk,t1,top]=%i2sci_g();txt=[txt;t1];
	end
      end
    else  // x(:)=y
      if sto(3)=='0'|sto(4)=='0' then
	if sfrom(4)=='1' then
	  txt=[txt;sto(1)+' = '+sfrom(1)+';']
	elseif isname(sfrom(1)) then
	  txt=[txt;sto(1)+' = '+sfrom(1)+'(:);']
	else
	  txt=[txt;sto(1)+' = '+sfrom(1)+';'+sto(1)+' = '+sto(1)+'(:);']
	end
      else
	txt=[txt;sto(1)+'(:) = '+sfrom(1)+';']
      end
      stk=list(op(2),'-1','?','1',sto(5))
    end
  end
else // x(i,j)=y
  s1=stk(top+1)
  if sto(5)=='10'|sfrom(5)=='10' then //insertion of strings
    txt=[txt;sto(1)+' = mtlb_is'+rhsargs([sto(1),sfrom(1),s2(1),s1(1)])]
    stk=list(op(2),'-1','?','?',sto(5))
  elseif s2(5)=='10'|s1(5)=='10' then
    txt=[txt;sto(1)+'('+s2(1)+')('+s1(1)+') = '+sfrom(1)+';']
    stk=list(op(2),'-1','?','?',sto(5))
  else
    txt=[txt;sto(1)+rhsargs([s2(1),s1(1)])+' = '+sfrom(1)+';']
    stk=list(op(2),'-1','?','?',sto(5))
  end
end

function [stk,txt,top]=%i2sci_rec()
n=s2(1)(1)
m=s2(1)(2)
ex=sto(1)

if m>1 then n=n-1,end
for k=1:n
  ik=s2(k+1)
  ex=ex+'('+ik(1)+')'
end
if m>1 then
  args=[]
  for k=1:m,args=[args s2(n+1+k)(1)],end
  ex=ex+'('+makeargs(args)+')'
end
txt=ex+'='+sfrom(1)
stk=list(' ','-1','?','1','?')

function [stk,txt,top]=%i2sci_rec2(s2,flag)
n=s2(1)(1)
m=s2(1)(2)
[lhs,rhs]=argn()
if rhs==1 then txt=sto(1),stk=list();else ,end

if m>1 then n=n-1,end
for k=1:n
  ik=s2(k+1)
  if type(ik(1))==1 then
    [stk,txt,top]=%i2sci_rec(ik,0)
  else
    txt=txt+'('+ik(1)+')'
  end
end
if m>1 then
  args=[]
  for k=1:m,args=[args s2(n+1+k)(1)],end
  txt=txt+'('+makeargs(args)+')'
end
if rhs==1 then 
  txt=txt+'='+sfrom(1)
  stk=list(' ','-1','?','1','?')
end



function [stk,txt,top]=%i2sci_s()
// x(i)=y with x a scalar
if sfrom(3)=='1'&sfrom(4)=='1' then
  txt=sto(1)+'(1,'+s2(1)+') = '+sfrom(1)
elseif sfrom(3)=='1' then
  txt=sto(1)+'(1,'+s2(1)+') = '+sfrom(1)
elseif sfrom(3)<>'?'& sfrom(4)<>'?' then
  txt=sto(1)+'(1,'+s2(1)+') = matrix('+sfrom(1)+',1,-1)'
else
  if ~isname(sfrom(1)) then
    w=gettempvar()
    txt=w+' = '+sfrom(1)
    sfrom(1)=w
  end
  txt=[txt;sto(1)+'(1,'+s2(1)+') = matrix('+sfrom(1)+',1,-1)']
end
stk=list(op(2),'-1','1','?',sto(5))

function [stk,txt,top]=%i2sci_r()
// x(i)=y with x  a row

if sfrom(3)=='1'&sfrom(4)=='1' then //insert a scalar
  txt=sto(1)+'('+s2(1)+') = '+sfrom(1)
elseif sfrom(3)=='1' then //insert a row vector
  txt=sto(1)+'('+s2(1)+') = '+sfrom(1) 
elseif sfrom(3)<>'?'& sfrom(4)<>'?' then //insert a matrix with known sizes
  txt=sto(1)+'('+s2(1)+') = matrix('+sfrom(1)+',1,-1)'
else
  if ~isname(sfrom(1)) then
    w=gettempvar()
    txt=w+' = '+sfrom(1)
    sfrom(1)=w
  end
  sfrom(1)=sfrom(1)+'(:).''',
  txt=[txt;sto(1)+'(1,'+s2(1)+') = '+sfrom(1)+';']
end
stk=list(op(2),'-1','1','?',sto(5))

function [stk,txt,top]=%i2sci_c()
//  x(i)=y with x  a column vector
if sfrom(3)=='1'&sfrom(4)=='1' then //insert a scalar
  txt=sto(1)+'('+s2(1)+') = '+sfrom(1)
elseif sfrom(4)=='1' then //insert a column vector
  txt=sto(1)+'('+s2(1)+') = '+sfrom(1) 
elseif sfrom(3)<>'?'& sfrom(4)<>'?' then //insert a matrix with known sizes
  sz=mulf(sfrom(3),sfrom(4))
  txt=sto(1)+'('+s2(1)+') = matrix('+sfrom(1)+',-1,1)'
else
  if ~isname(sfrom(1)) then
    w=gettempvar()
    txt=w+' = '+sfrom(1)
    sfrom(1)=w
  end
  sfrom(1)=sfrom(1)+'(:)',
  txt=[txt;sto(1)+'('+s2(1)+',1) = '+sfrom(1)+';']
end
stk=list(op(2),'-1','?','1',sto(5))

function [stk,txt,top]=%i2sci_g()
//  x(i)=y with x a matrix
e=sto(1)+' = mtlb_i('+sto(1)+','+s2(1)+','+sfrom(1)+')'
e1=sto(1)+'('+s2(1)+') = '+sfrom(1)
set_infos([
    e+' may be replaced by '+e1
    '  if '+sto(1)+' is not a  vector'
    '  or if '+sto(1)+' and '+sfrom(1)+' are both row or column vectors'],1)
txt=[txt; e]
stk=list(op(2),'-1','?','?',sto(5))


