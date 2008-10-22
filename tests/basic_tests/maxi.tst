// Copyright INRIA
Ntest=1
// test maxi(A) : A matrix 
// maxi is compared with sort 

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=maxi(a); [a1,ak1]=sort(a); 
	if norm([am-a1(1,1),(ak(1)+(m)*(ak(2)-1))-ak1(1,1)])> %eps then pause,end
end

// test maxi(A) : A vector

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=maxi(a); [a1,ak1]=sort(a); 
	if norm([am-a1(1,1),(ak-ak1(1,1))])> %eps then pause,end
end

// test mini 

for i=1:Ntest,
	m=100;n=200;a=rand(m,n);
	[am,ak]=maxi(-a); [ami,aki]=mini(a);
	if norm([am+ami,ak-aki])> %eps then pause,end
end

for i=1:Ntest,
	m=100;a=rand(1,m);
	[am,ak]=maxi(-a); [ami,aki]=mini(a);
	if norm([am+ami,ak-aki])> %eps then pause,end
end


// test maxi(A1,A....) compared with usual maxi + feval

m=10,n=20;
A1=rand(m,n);
A2=rand(m,n);
A3=rand(m,n);
A4=rand(m,n);
deff('[y]=f(i,j)','[y,k]=maxi([A1(i,j),A2(i,j),A3(i,j),A4(i,j)]);y=y+%i*k');
A=feval(1:m,1:n,f);
Am=real(A);Ak=imag(A);
[Am1,Ak1]=maxi(A1,A2,A3,A4);

if norm(Am1-Am)> %eps then pause,end
if norm(Ak1-Ak)> %eps then pause,end

// test maxi(list(A1,A2,..)) compared to maxi(A1,..)

[Al,Akl]=maxi(list(A1,A2,A3,A4));
if norm(Al-Am)> %eps then pause,end
if norm(Akl-Ak)> %eps then pause,end

// test maxi(A,'c') and maxi(A,'r') 
// maxi(A,'r') returns a row vector which contains max for each column

[Am,Akm]=maxi(A1,'r');
nc=size(A1,'c')
Am1=[];Ak1=[]
for i=1:nc,[am,ak]=maxi(A1(:,i)); Am1=[Am1,am];Ak1=[Ak1,ak];end
if norm(Am-Am1)> %eps then pause,end
if norm(Akm-Ak1)> %eps then pause,end


// maxi(A,'c');

[Am,Akm]=maxi(A1,'c');
nc=size(A1,'r')
Am1=[];Ak1=[]
for i=1:nc,[am,ak]=maxi(A1(i,:)); Am1=[Am1;am];Ak1=[Ak1;ak];end
if norm(Am-Am1)> %eps then pause,end
if norm(Akm-Ak1)> %eps then pause,end
