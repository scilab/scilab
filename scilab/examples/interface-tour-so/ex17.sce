e1=int(10*rand(2,2));
e2=int(10*rand(3,3));
e3='ex07c';
e4=rand(2,2);
e5=rand(3,3);
e5=e5+rand(e5)*%i;
L=list(e1,e2,e3,e4,e5);

L1=ex17c(L);

for i=[1,2,4,5] 
 if norm(L1(i)-L(i)) > 1.e-4  then pause,end
end 
if L1(3)<>L(3) then pause,end 

e1 = e1 +%i*int(10*rand(e1));
L=list(e1,e2,e3,e4,e5);

L1=ex17c(L);


for i=[1,2,4,5] 
 if norm(L1(i)-L(i)) > 1.e-4  then pause,end
end 

if L1(3)<>L(3) then pause,end 
