function f1=%r_m_s(f1,n2)
// %r_m_s(f1,n2)  
//f1 =  f1*n2
//!
// Copyright INRIA
[n1,d1]=f1(['num','den']),
[l1,m1]=size(n1);[l2,m2]=size(n2),
if l1*m1==0|l2*m2==0 then,f1=[];return;end

indef=%f
if l1==-1 then 
  n1=n1+0;d1=d1+0;l1=1;m1=1;
  if l2*m2==1 then indef=%t,else error(14),end
end
if l2==-1 then 
  n2=n2+0;l2=1;m2=1;
  if l1*m1==1 then indef=%t,else error(14),end
end 
//
if mini([l1*m1,l2*m2])==1 then,
  num=n1*n2,
  den=d1*ones(l2,m2),
else,
  if m1<>l2 then error(10),end,
  for i=1:l1,
    n=n1(i,:);
    [y,fact]=lcm(d1(i,:)),
    den(i,1:m2)=ones(1,m2)*y,
    for j=1:m2,
      num(i,j)=n*(n2(:,j).*matrix(fact,l2,1)),
    end,
  end,
end,
[num,den]=simp(num,den),
if indef then
  num=num*eye()
  den=den*eye()
end
f1=rlist(num,den,f1('dt'))
endfunction
