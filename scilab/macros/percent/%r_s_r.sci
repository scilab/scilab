function f=%r_s_r(s1,s2)
//f=s1-s2 s1,s2 : rational matrices
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2),
[num1,den1]=s1(['num','den']),
[num2,den2]=s2(['num','den']),
[m1,n1]=size(num1)
[m2,n2]=size(num2)

if m1==-1 then
  num1=num1+0
  den1=den1+0
  n1=1
end
if m2==-1 then
  num2=num2+0
  den2=den2+0
  n2=1
end

if m1==-1&m2==-1 then
  // both are eye*x
  [den1,fact]=lcm([den1;den2])
  [num1,den1]=simp([num1,-num2]*fact,den1)
  f=rlist(num1*eye(),den1*eye(),s1('dt')),
  return
end
m1=abs(m1);m2=abs(m2)
if m1*n1==1&m2*n2>1 then
  //scalar +matrix:replace scalar by scalar*ones(matrix)
  num1=num1*ones(num2)
  den1=den1*ones(num2)
  m1=m2
  n1=n2
elseif m2*n2==1&m1*n1>1 then
  // matrix+scalar:replace scalar by scalar*ones(matrix)
  num2=num2*ones(num1)
  den2=den2*ones(den1)
  m2=m1
  n2=n1
end

if m1<>m2|n1<>n2 then error(9),end

for l=1:m1
  for k=1:n1
    [den,fact]=lcm([den1(l,k);den2(l,k)])
    num1(l,k)=[num1(l,k),-num2(l,k)]*fact
    den1(l,k)=den
  end,
end,
[num1,den1]=simp(num1,den1),
f=rlist(num1,den1,s1('dt'))
endfunction
