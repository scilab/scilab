function f2=%s_m_r(n1,f2)
// %s_m_r(n1,f2)  
//operation  f2=n1*f2
//!
// Copyright INRIA
[n2,d2]=f2(['num','den']);
[l1,m1]=size(n1);[l2,m2]=size(n2),
if l1*m1==0|l2*m2==0 then
  f2=[]
  return
end
indef=%f
if l1==-1 then 
  n1=n1+0;l1=1;m1=1;
  if l2*m2==1 then indef=%t,else error(14),end
end
if l2==-1 then 
  n2=n2+0;d2=d2+0;l2=1;m2=1;
  if l1*m1==1 then indef=%t,else error(14),end
end  

//
if mini([l1*m1,l2*m2])==1 then,
  num=n1*n2,
  den=ones(l1,m1)*d2,
else,
 if m1<>l2 then error(10),end,
  for j=1:m2,
    [y,fact]=lcm(d2(:,j)),
    n2(:,j)=n2(:,j).*fact,
    den(1:l1,j)=ones(l1,1)*y,
    for i=1:l1,
      num(i,j)=n1(i,:)*n2(:,j),
    end,
  end,
end,
[num,den]=simp(num,den),
if indef then
  num=num*eye()
  den=den*eye()
end
f2=rlist(num,den,f2('dt'))
endfunction
