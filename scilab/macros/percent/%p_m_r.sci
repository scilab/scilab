function f=%p_m_r(p,f)
// r=%p_m_r(p,f)  <=> r= p*f with p=polynomial matrix
// and f= rational
//!
// Copyright INRIA
[n2,d2]=f(['num','den']);
[l1,m1]=size(p);[l2,m2]=size(n2);
//
indef=%f
if l1==-1 then 
  p=p+0;l1=1;m1=1;
  if l2*m2==1 then indef=%t,else error(14),end
end
if l2==-1 then 
  n2=n2+0;d2=d2+0;l2=1;m2=1;
  if l1*m1==1 then indef=%t,else error(14),end
end  

if mini([l1*m1,l2*m2])==1 then
  num=p*n2,
  den=ones(l1,m1)*d2,
else

  if m1<>l2 then error(10),end
  //
  for j=1:m2,
    y=lcm(d2(:,j))
    for i=1:l1,
      x=0;
      for k=1:m1,
	x=x+p(i,k)*pdiv(y,d2(k,j))*n2(k,j),
      end
      num(i,j)=x,den(i,j)=y,
    end
  end
end
[num,den]=simp(num,den)
if indef then
  num=num*eye()
  den=den*eye()
end
f=rlist(num,den,f('dt'))
endfunction
