function kp=krac2(n)
// Copyright INRIA
if type(n)<>16 then error(97,1),end;
flag=n(1)
select flag(1)
case 'r' then [n,d,dom]=n(2:4)
case 'lss' then n=ss2tf(n);[n,d,dom]=n(['num','den','dt'])
else error(97,1),
end;
if dom<>'c' then error('System must be continuous'),end
if size(n,'*')<>1 then error(95,1),end

x=[];
q1=derivat(n/d);s=roots(q1(2));
//
for a=s',
  if abs(imag(a))<=10*%eps then 
    x=[x;a],
  end,
end
//x(x==0)=[]
if x==[] then;return,end
kp=sort(-real(freq(d,n,real(x))))
endfunction
