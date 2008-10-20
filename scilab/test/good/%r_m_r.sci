function s1=%r_m_r(s1,s2)
//s1=s1*s2
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2),
[n1,d1]=s1(['num','den']);[n2,d2]=s2(['num','den']);
[l1,m1]=size(n1);[l2,m2]=size(n2),
indef=%f
if l1==-1 then 
  n1=n1+0;d1=d1+0;l1=1;m1=1;
  if l2*m2==1 then indef=%t,else error(14),end
end
if l2==-1 then 
  n2=n2+0;d2=d2+0;l2=1;m2=1;
  if l1*m1==1 then indef=%t,else error(14),end
end  
if mini([l1*m1,l2*m2])==1 then
  num=n1*n2
  den=d1*d2
else
  if m1<>l2 then error(10),end
  for i=1:l1, pp(i)=lcm(d1(i,:)),end,
  for j=1:m2,
    y=lcm(d2(:,j)),
    for i=1:l1,
      yij=y*pp(i),
      x=0;
      for k=1:m1,
	x=x+n1(i,k)*n2(k,j)*pppdiv(y,d2(k,j))*pppdiv(pp(i),d1(i,k)),
      end
      num(i,j)=x,den(i,j)=yij,
    end
  end
end
[num,den]=simp(num,den),
if indef then
  num=num*eye()
  den=den*eye()
end
s1=rlist(num,den,s1('dt'))
endfunction
