// Copyright INRIA
pi=%pi
i=%i
e=%e
//       tests
//
1
a=1
b=[1 2 3]
c=[1 2 3;4 5 6]
d=[1 2 3;4 5 6]'
d=[1 2 3 4;4 5 6 7;8 9 10 11;12 13 14 15]
d=[1 2;3 4]
e1=[[1 2] [3 4];[5 6 7 8];[9;10;11;12]']
f([1 3 5],[1 2 3])=[-1 -2 -3;-4 -5 -6;-7 -8 -9]
g=[i,2,3;1 i 3;1 2 i]
h=[i 1 2 i 3]
o=[1 i i 2 3]'
b(2)=3
e1(1,3)=1
p='apcdefghijklmnopqrstuvwxyz0123456789'
// test de stackg
a,b,c,d,e1,f,g,h,o,p
b(2)
e1(1,3)
e1([1 2],[3 4])
e1(1:2,:)
e1(:,4)
e1(:,:)
g(1,1)
g(:,1)
g(1:2,:)
g(:,:)
o'
//test de stack2
//
// additions...
[1 2 3;4 5 6]-[1 2 3;4 5 6]
[1 2 3;4 5 6]+2*[1 2 3;4 5 6]-[1 2 3;4 5 6]*3
[1 i 3;4 5 i]+2*[1 i 3;4 5 i]-[1 i 3;4 5 i]*3
[2*i -4*i 2*i]+2*[i -2*i i]+i*[2 -4 2]-[i -2*i i]*2 -2*[2 -4 2]*i
//
2\[4 8 16]/2-[1 2 4]
i\[2*i 2 i*4]/i-[-i*2 -2 -4*i]
// element wise operations
[1 2;3 4;5 6].*[1 2;3 4;5 6]-[1 4;9 16;25 36]
[1 2;3 4;5 6*i].*[1 2;3 4;5 6]-[1 4;9 16;25 36*i]
[1 2;3 4;5 6].*[1 2;3 4;5 6*i]-[1 4;9 16;25 36*i]
[1 2;3 4;5 6*i].*[1 2;3 4;5 6*i]-[1 4;9 16;25 -36]
//
[2 9 8;3 10 15]./[2 3 4;3 5 5]-[1 3 2;1 2 3]
[2 9*i 8;3 10 15]./[2 3 4;3 5 5]-[1 3*i 2;1 2 3]
[2 9 8;3 10 15]./[2*i 3 4;3 5 5]-[-i 3 2;1 2 3]
[2*i 9 8;3 10 15]./[2*i 3 4;3 5 5]-[1 3 2;1 2 3]
//
[2 3 4;3 5 5].\[2 9 8;3 10 15]-[1 3 2;1 2 3]
[2 i 4;3 5 5].\[2 9 8;3 10 15]-[1 -9*i 2 ;1 2 3]
[2 3 4;3 5 5].\[i 9 8;3 10 15]-[.5*i 3 2;1 2 3]
[i 3 4;3 5 5].\[i 9 8;3 10 15]-[1 3 2;1 2 3]
//multiplication
[1 2 3;4 5 6]*[3;2;1]-[10;28]
[1 i 3;4 5 6]*[3;2;1]-[6+2*i;28]
[1 2 3;4 5 6]*[i;2;1]-[i+7;4*i+16]
[1 i 3;2*i,-i,1]*[i;-i;i]-[4*i+1;-3+i]
//eye
eye(4,4)
//a+-b*eye a*eye+-b a+-eye*b eye*a+-b
-2*eye()+[1 2;3 4]+eye()*2-[1 2;3 4]
-2*i*eye()+[1 2;3 4]+eye()*2*i-[1 2;3 4]
// :
1:10
1:.1:2
// for
for k=1:3,for l=1:2,a(k,l)=k+l;end;end;a
diag([1 2 3])-[1 0 0;0 2 0;0 0 3]
diag([1 i 2])-[1 0 0;0 i 0;0 0 2]
a=[1 2 3 4;5 6 7 8];
c=a;
c(1,1)=i;
diag(a)-[1;6]
diag(a,1)-[2;7]
diag(a,-1)-[5]
diag(a,4)
diag(c)-[i;6]
diag(c,1)-[2;7]
diag(c,-1)-[5]
diag(c,4)
//
eye(a)
eye(c)
eye(3,3)
eye(2,3)
//
ones(a)
ones(c)
ones(3,3)
ones(3,2)
//
rand(a)
rand(c)
rand(3,3)
rand(3,2)
rand()
rand('uniform')
rand('normal')
rand('seed',5)
rand('seed',0)
//
abs(c)
abs(a)
//
real(c)
imag(c)
real(a)
imag(a)
//
round(rand(3,3))
//
conj(a)
conj(c)
//
size(a)
[m,n]=size(a)
//
triu(a)
tril(a)
triu(a,1)
triu(a,-1)
tril(a,1)
tril(a,-1)
triu(c)
tril(c)
triu(c,1)
triu(c,10)
triu(c,-1)
triu(c,-10)
tril(c,1)
tril(c,10)
tril(c,-1)
tril(c,-10)
//test de matlu
a=rand(4,4);b=rand(5,4);ac=a+i*rand(4,4);bc=b+i*rand(5,4);
//
if abs((1/a)*a-eye())> 10*%eps then pause,end
if abs((i/a)*a-i*eye())> 10*%eps then pause,end
if abs((1/ac)*ac-eye())> 10*%eps then pause,end
if abs((i/ac)*ac-i*eye())> 10*%eps then pause,end
if abs(a*(a\1)-eye())> 10*%eps then pause,end
if abs(a*(a\i)-i*eye())> 10*%eps then pause,end
if abs(ac*(ac\1)-eye())> 10*%eps then pause,end
if abs(ac*(ac\i)-eye()*i)> 10*%eps then pause,end
//
if abs(inv(a)*a-eye())> 10*%eps then pause,end
if abs(inv(ac)*ac-eye())> 10*%eps then pause,end
//
if abs((b/a)*a-b)> 10*%eps then pause,end
if abs((b/ac)*ac-b)> 10*%eps then pause,end
if abs((bc/a)*a-bc)> 10*%eps then pause,end
if abs((bc/ac)*ac-bc)> 10*%eps then pause,end
//
if abs(a*(a\b')-b')> 10*%eps then pause,end
if abs(ac*(ac\b')-b')> 10*%eps then pause,end
if abs(a*(a\bc')-bc')> 10*%eps then pause,end
if abs(ac*(ac\bc')-bc')> 10*%eps then pause,end
//
[l u]=lu(a);
if abs(l*u-a)> 10*%eps then pause,end
[l u]=lu(ac);
if abs(l*u-ac)> 10*%eps then pause,end
//
h1(5,5)=0;for k=1:5,for l=1:5, h1(k,l)=1/(k+l-1);end;end;
if abs(inv(h1)-testmatrix('hilb',5))> 1.e-7 then pause,end
//
if abs(det(testmatrix('magic',5))-5070000)> 1.e-7 then pause,end
//
b=a*a';h=chol(b);
if abs(h'*h-b)> 10*%eps then pause,end
bc=triu(ac*ac');bc=bc+bc'-diag(real(diag(bc)));;h=chol(bc);
if abs(h'*h-bc)> 10*%eps then pause,end
//test de matqr
a=rand(3,4);b=rand(3,4);ac=a+i*rand(3,4);bc=b+i*rand(3,4);
//
if abs(a*(1/a)*a-a)> 10*%eps then pause,end
if abs(a*(i/a)*a-i*a)> 10*%eps then pause,end
if abs(a*(a\ 1)*a-a)> 10*%eps then pause,end
if abs(a*(a\ i)*a-i*a)> 10*%eps then pause,end
if abs(ac*(1/ac)*ac-ac)> 10*%eps then pause,end
if abs(ac*(i/ac)*ac-i*ac)> 10*%eps then pause,end
if abs(ac*(ac\ 1)*ac-ac)> 10*%eps then pause,end
if abs(ac*(ac\ i)*ac-i*ac)> 10*%eps then pause,end
//
if abs(a/b-a*(1/b))> 10*%eps then pause,end
if abs(ac/b-ac*(1/b))> 10*%eps then pause,end
if abs(a/bc-a*(1/bc))> 10*%eps then pause,end
if abs(ac/bc-ac*(1/bc))> 10*%eps then pause,end
//
if abs(a\ b -(a\ 1)*b)> 10*%eps then pause,end
if abs(a\ bc-(a\ 1)*bc)> 10*%eps then pause,end
if abs(ac\ b-(ac\ 1)*b)> 10*%eps then pause,end
if abs(ac\ bc-(ac\ 1)*bc)> 10*%eps then pause,end
//
//elemt-wise
a=rand(3,2);ai=a+rand(3,2)*%i;
de=2;
if norm(ai.*de-ai*de,1) >1000*%eps then pause,end
if norm(de.*ai-de*ai,1) >1000*%eps then pause,end
if norm(ai.*de-ai*de,1) >1000*%eps then pause,end
if norm(de.*ai-de*ai,1) >1000*%eps then pause,end
de=de+3*%i;
if norm(ai.*de-ai*de,1) >1000*%eps then pause,end
if norm(de.*ai-de*ai,1) >1000*%eps then pause,end
if norm(ai.*de-ai*de,1) >1000*%eps then pause,end
if norm(de.*ai-de*ai,1) >1000*%eps then pause,end
//
de=2;def=de*ones(a);
if norm(a./de-a./def,1) >1000*%eps then pause,end
if norm(ai./de-ai./def,1) >1000*%eps then pause,end
//
de=2+3*%i;def=de*ones(a);
if norm(a./de-a./def,1) >1000*%eps then pause,end
if norm(ai./de-ai./def,1) >1000*%eps then pause,end
de=2;def=de*ones(a);
if norm(a.\de-a.\def,1) >1000*%eps then pause,end
if norm(ai.\de-ai.\def,1) >1000*%eps then pause,end
//
de=2+3*%i;def=de*ones(a);
if norm(a.\de-a.\def,1) >1000*%eps then pause,end
if norm(ai.\de-ai.\def,1) >1000*%eps then pause,end

///////////
de=2;def=de*ones(a);
if norm(de.\a-de.\a,1) >1000*%eps then pause,end
if norm(de.\ai-def.\ai,1) >1000*%eps then pause,end
//
de=2+3*%i;def=de*ones(a);
if norm(de.\a-def.\a,1) >1000*%eps then pause,end
if norm(de.\ai-def.\ai,1) >1000*%eps then pause,end
de=2;def=de*ones(a);
if norm(de./a-def./a,1) >1000*%eps then pause,end
if norm(de./ai-def./ai,1) >1000*%eps then pause,end
//
de=2+3*%i;def=de*ones(a);
if norm(de./a-def./a,1) >1000*%eps then pause,end
if norm(de./ai-def./ai,1) >1000*%eps then pause,end
//
[p,r]=qr(a);
if abs(p*r-a)> 10*%eps then pause,end
[p,r]=qr(a');
if abs(p*r-a')> 10*%eps then pause,end
[p,r,x]=qr(a);
if abs(p*r*x'-a)> 10*%eps then pause,end
[p,r]=qr(ac);
if abs(p*r-ac)> 10*%eps then pause,end
[p,r,x]=qr(ac);
if abs(p*r-ac*x)> 10*%eps then pause,end
//
if abs(cond(diag([1 2 3 4]))-4)> 10*%eps then pause,end
if abs(cond(diag([1 i 3 4]))-4)> 10*%eps then pause,end
v=[1 2 3 4 5];
if abs(norm(v,1)-15)> 10*%eps then pause,end
if abs(norm(v,'inf')-5)> 10*%eps then pause,end
if abs(norm(v,2)-sqrt(55))> 10*%eps then pause,end
if abs(norm(v,'fro')-sqrt(55))> 10*%eps then pause,end
v=v';
if abs(norm(v,1)-15)> 10*%eps then pause,end
if abs(norm(v,'inf')-5)> 10*%eps then pause,end
if abs(norm(v,2)-sqrt(55))> 10*%eps then pause,end
if abs(norm(v,'fro')-sqrt(55))> 10*%eps then pause,end
v=[i 2 3 4 5];
if abs(norm(v,'inf')-5)> 10*%eps then pause,end
if abs(norm(v,2)-sqrt(55))> 10*%eps then pause,end
if abs(norm(v,'fro')-sqrt(55))> 10*%eps then pause,end
v=v';
if abs(norm(v,1)-15)> 10*%eps then pause,end
if abs(norm(v,'inf')-5)> 10*%eps then pause,end
if abs(norm(v,2)-sqrt(55))> 10*%eps then pause,end
if abs(norm(v,'fro')-sqrt(55))> 10*%eps then pause,end
a=[diag([1 2 3]);[0 0 0]];
if abs(norm(a,1)-3)> 10*%eps then pause,end
if abs(norm(a,'inf')-3)> 10*%eps then pause,end
if abs(norm(a,2)-3)> 10*%eps then pause,end
if abs(norm(a,'fro')-sqrt(14))> 10*%eps then pause,end
a=a';
if abs(norm(a,1)-3)> 10*%eps then pause,end
if abs(norm(a,'inf')-3)> 10*%eps then pause,end
if abs(norm(a,2)-3)> 10*%eps then pause,end
if abs(norm(a,'fro')-sqrt(14))> 10*%eps then pause,end
a=[diag([i,2,3]),[0;0;0]];
if abs(norm(a,'inf')-3)> 10*%eps then pause,end
if abs(norm(a,2)-3)> 10*%eps then pause,end
if abs(norm(a,'fro')-sqrt(14))> 10*%eps then pause,end
a=a';
if abs(norm(a,1)-3)> 10*%eps then pause,end
if abs(norm(a,'inf')-3)> 10*%eps then pause,end
if abs(norm(a,2)-3)> 10*%eps then pause,end
if abs(norm(a,'fro')-sqrt(14))> 10*%eps then pause,end
//
a=rand(3,5);ac=a+i*rand(3,5);
[u,s,v]=svd(a);u*s*v'-a;
if abs(svd(a)-diag(s))> 10*%eps then pause,end
[u,s,v]=svd(ac);u*s*v'-ac;
if abs(svd(ac)-diag(s))> 10*%eps then pause,end
//
[u,s,v]=svd(a,0);u*s*v'-a;
if abs(svd(a,0)-diag(s,0))> 10*%eps then pause,end
[u,s,v]=svd(ac,0);u*s*v'-ac;
if abs(svd(ac,0)-diag(s))> 10*%eps then pause,end
a=a';ac=ac';
a=rand(3,5);ac=a+i*rand(3,5);
[u,s,v]=svd(a);u*s*v'-a;
if abs(svd(a)-diag(s))> 10*%eps then pause,end
[u,s,v]=svd(ac);u*s*v'-ac;
if abs(svd(ac)-diag(s))> 10*%eps then pause,end
//
[u,s,v]=svd(a,0);u*s*v'-a;
if abs(svd(a,0)-diag(s,0))> 10*%eps then pause,end
[u,s,v]=svd(ac,0);u*s*v'-ac;
if abs(svd(ac,0)-diag(s))> 10*%eps then pause,end
//
if abs(a*pinv(a)*a-a)> 10*%eps then pause,end
if abs(ac*pinv(ac)*ac -ac)> 10*%eps then pause,end
a=a';ac=ac';
if abs(a*pinv(a)*a-a)> 10*%eps then pause,end
if abs(ac*pinv(ac)*ac-ac)> 10*%eps then pause,end
//
if abs(rank(a)-3)> 10*%eps then pause,end
if abs(rank(ac)-3)> 10*%eps then pause,end
rand('seed',0)
// matdsr  matdsc
a=rand(4,4);ac=a+i*rand(4,4);t=a*a';tc=ac*ac';
tc=triu(tc,1)+triu(tc,1)'+diag(real(diag(tc)));
tc=tc/max(abs(spec(tc))+1);

// fonctions matricielles
s=sqrtm(t);
if norm(t-s*s,1) > 500*%eps then pause,end
s=logm(t);
if norm(t-expm(s)) > 500*%eps then pause,end
s=sqrtm(tc);
if norm(tc-s*s,1) > 500*%eps then pause,end
s=expm(tc);s=triu(s,1)+triu(s,1)'+diag(real(diag(s)));
if norm(logm(s)-tc,1)> 500*%eps then pause,end
if norm(sinm(t)**2+cosm(t)**2-eye(),1) > 50*%eps then pause,end
if norm(sinm(tc)**2+cosm(tc)**2-eye(),1) > 50*%eps then pause,end
//poly et root
p=rand(5,1);pc=p+i*rand(5,1);x=poly(0,'x');
if norm(sort(p )-sort(real(roots(poly(p,'x'))))) > 1000*%eps then pause,end
if norm(sort(imag(pc))-sort(real(roots(poly(imag(pc),'x'))))) > 10000*%eps then pause,end
//**
if norm(t^(-1)-inv(t),1) > 200*%eps then pause,end
if norm(tc**(-1)-inv(tc),1) > 200*%eps then pause,end 
x=rand()+i*rand();
if norm((t**x)*(t^(-x))-eye(),1) > 200*%eps then pause,end 
if norm((tc**x)*(tc**(-x))-eye(),1) > 200*%eps then pause,end
//element op
if norm(sin([0 pi/2 pi 3*pi/2])-[0 1 0 -1],1) > 10*%eps then pause,end
if norm(cos([0 pi/2 pi 3*pi/2])-[1 0 -1 0],1) > 10*%eps then pause,end
if norm(log([e 1/e e**2])-[1 -1 2],1) > 10*%eps then pause,end
if norm(exp([1 -1 2])-[e 1/e e**2],1) > 10*%eps then pause,end
if norm(sqrt([1 -1 4])-[1 i 2],1) > 10*%eps then pause,end
if norm(sin(x)-(exp(i*x)-exp(-i*x))/(2*i),1) > 10*%eps then pause,end
if norm(cos(x)-(exp(i*x)+exp(-i*x))/2,1) > 10*%eps then pause,end
if norm(sqrt(x)^2-x,1) > 10*%eps then pause,end
if norm(log(exp(x))-x,1) > 10*%eps then pause,end
// triu
z=poly(0,'z');a=[z 1 -z+1  8;z*z 10*z 5 -z;3 7 z+1 -1-z];
[m,n]=size(a);mn=mini([m n]);a1=a;l=1;
//
for dg=-(m-1):0,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   for k=1:l,a1(-dg+k,k)=0;end,l=mini([l+1,mn]);end
for dg=1:n,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a';a1=a;[m,n]=size(a);mn=mini([m,n]);l=1;
for dg=-(m-1):0,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   for k=1:l,a1(-dg+k,k)=0;end,l=mini([l+1,mn]);end
for dg=1:n,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a+%i*[1 2 3 4;5 6 7 8;9 10 11 12]';
[m,n]=size(a);mn=mini([m n]);a1=a;l=1;
//
for dg=-(m-1):0,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   for k=1:l,a1(-dg+k,k)=0;end,l=mini([l+1,mn]);end
for dg=1:n,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;
//
a=a';a1=a;[m,n]=size(a);mn=mini([m,n]);l=1;
for dg=-(m-1):0,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   for k=1:l,a1(-dg+k,k)=0;end,l=mini([l+1,mn]);end
for dg=1:n,
   if norm(coeff(triu(a,dg)-a1),1)>%eps then dg,pause,end,
   if dg>(n-m),l=l-1;end;for k=1:l,a1(k,dg+k)=0;end;end;

//test de clear
x=[1,2,3];
clear x,if exists('x')==1 then pause,end
x=[1,2,3];y='afs';
clear x y,if exists('x')==1| exists('y')==1 then pause,end
X=[1,2,3];Y='afs';
clear X Y,if exists('X')==1| exists('Y')==1 then pause,end


//test with scalar vector and matrices

// REAL VECTORS
x1=1:3;
x0=0:2;
if or(x1^2<>[1 4 9]) then pause,end
if or(x0^2<>[0 1 4]) then pause,end
if or(x1^0<>[1 1 1]) then pause,end

if norm(x1^(2*(1+%eps))-x1^2)>100*%eps then pause,end
if norm(x0^(2*(1+%eps))-x0^2)>100*%eps then pause,end

if norm(x1^(-2)-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1^(-2*(1+%eps))-[1 0.25 1/9])>100*%eps then pause,end

// COMPLEX CASE
p=2+%eps*%i;
if norm(x1^p-x1^2)>100*%eps then pause,end
if norm(x0^p-x0^2)>100*%eps then pause,end
if norm(x1^(-p)-x1^(-2))>100*%eps then pause,end

y=%eps*%eps*ones(1,3);x1=x1+y;x0=x0+y;
if norm(x1^2-[1 4 9])>100*%eps then pause,end
if norm(x0^2-[0 1 4])>100*%eps then pause,end

x1^2.000000001;x0^2.000000001;
if norm(x1^2-[1 4 9])>100*%eps then pause,end
if norm(x0^2-[0 1 4])>100*%eps then pause,end

if norm(x1^(-2)-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1^(-2*(1+%eps))-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1^p-x1^2)>100*%eps then pause,end
if norm(x0^p-x0^2)>100*%eps then pause,end
if norm(x1^(-p)-x1^(-2))>100*%eps then pause,end

// .^ TEST WITH  REAL VECTORS

x1=1:3;
x0=0:2;
if or(x1.^2<>[1 4 9]) then pause,end
if or(x0.^2<>[0 1 4]) then pause,end
if or(x1.^0<>[1 1 1]) then pause,end

if norm(x1.^(2*(1+%eps))-x1.^2)>100*%eps then pause,end
if norm(x0.^(2*(1+%eps))-x0.^2)>100*%eps then pause,end

if norm(x1.^(-2)-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1.^(-2*(1+%eps))-[1 0.25 1/9])>100*%eps then pause,end

// .^ TEST WITH  COMPLEX VECTORS

p=2+%eps*%i;
if norm(x1.^p-x1.^2)>100*%eps then pause,end
if norm(x0.^p-x0.^2)>100*%eps then pause,end
if norm(x1.^(-p)-x1.^(-2))>100*%eps then pause,end

y=%eps*%eps*ones(1,3);x1=x1+y;x0=x0+y;
if norm(x1.^2-[1 4 9])>100*%eps then pause,end
if norm(x0.^2-[0 1 4])>100*%eps then pause,end

x1.^2.000000001;x0.^2.000000001;
if norm(x1.^2-[1 4 9])>100*%eps then pause,end
if norm(x0.^2-[0 1 4])>100*%eps then pause,end

if norm(x1.^(-2)-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1.^(-2*(1+%eps))-[1 0.25 1/9])>100*%eps then pause,end
if norm(x1.^p-x1.^2)>100*%eps then pause,end
if norm(x0.^p-x0.^2)>100*%eps then pause,end
if norm(x1.^(-p)-x1.^(-2))>100*%eps then pause,end
//
// EMPTY MATRIX
//=============
if []^1 <> [] then pause,end
if []^[1 2 3] <> [] then pause,end
if [1 2 3]^[] <> [] then pause,end
if []^[] <> [] then pause,end

// SQUARE MATRIX
//==============

x1=[1 2;3 4];
if or(x1^1<>x1) then pause,end
if or(x1^(-1)<>inv(x1)) then pause,end
if or(x1^2<>x1*x1) then pause,end
if or(x1^(-2)<>inv(x1)^2) then pause,end

x1(1,1)=x1(1,1)+%i;
if or(x1^2<>x1*x1) then pause,end
if or(x1^(-2)<>inv(x1)^2) then pause,end
if or(x1^1<>x1) then pause,end
if or(x1^(-1)<>inv(x1)) then pause,end

if or(rand(4,4)^0<>eye(4,4)) then pause,end


x1=[1 2;2 3];x2=x1^(1/2);
if norm(x2^(2)-x1)>100*%eps then pause,end
x2=x1^(-1/2);
if norm(x2^(-2)-x1)>100*%eps then pause,end

x1=[1 2+%i;2-%i 3];
x2=x1^(1/2);
if norm(x2^(2)-x1)>100*%eps then pause,end
x2=x1^(-1/2);
if norm(x2^(-2)-x1)>100*%eps then pause,end

//TEST WITH POLYNOMIAL VECTOR AND MATRICES
//---------------------------------------
s=poly(0,'s');
if or(coeff(s^3+1)<>[1 0 0 1]) then pause,end


x1=[1 s+1 s^3+1];
if or(x1^2<>[1 1+2*s+s^2  1+2*s^3+s^6]) then pause,end
if or(coeff(x1^0)<>[1 1 1]) then pause,end
if or(x1^3<>[1,1+3*s+3*s^2+s^3,1+3*s^3+3*s^6+s^9]) then pause,end
if or((x1^(-1)-[1 1/(1+s)  1/(1+s^3)])<>0) then pause,end

x1=[1 s+1 s.^3+1];
if or(x1.^2<>[1 1+2*s+s.^2  1+2*s.^3+s.^6]) then pause,end
if or(coeff(x1.^0)<>[1 1 1]) then pause,end
if or(x1.^3<>[1,1+3*s+3*s^2+s^3,1+3*s^3+3*s^6+s^9]) then pause,end
if or((x1.^(-1)-[1 1/(1+s)  1/(1+s.^3)])<>0) then pause,end




x1=[s+1 2*s;3+4*s^2 4];
if or(x1^1<>x1) then pause,end
if or(x1^(-1)<>inv(x1)) then pause,end
if or(x1^2<>x1*x1) then pause,end
if or(x1^(-2)<>inv(x1)^2) then pause,end

x1(1,1)=x1(1,1)+%i;
if or(x1^2<>x1*x1) then pause,end
//if or(x1^(-2)<>inv(x1)^2) then pause,end  //simp complexe non implemented
if or(x1^1<>x1) then pause,end
//if or(x1^(-1)<>inv(x1)) then pause,end //simp complexe non implemented


//simple ops
if 1==0 then pause,end
if 1<0 then pause,end
if 1<=0 then pause,end
if 1==1 then,else pause,end
if ~(1==1) then pause,end
if 0>1 then pause,end
if 0>=1 then pause,end
if 1<>1 then pause,end
if 0<>1 then,else pause,end
if '1'=='0' then pause,end
if '1'<>'1' then pause,end

%s=poly(0,'s');
if %s==0 then pause,end
if 0==%s then pause,end
if %s==%s then,else pause,end
if %s<>%s then pause,end
if %s==%s+1 then pause,end
if %s+1==%s then pause,end

if 1/%s==0 then pause,end
if 0==1/%s then pause,end
if 1/%s==1/%s then,else pause,end
if 1/%s<>1/%s then pause,end
if 1/%s==1/%s+1 then pause,end
if 1/%s+1==1/%s then pause,end
if 1/%s<>1/%s+1 then , else  pause,end
if 1/%s+1<>1/%s then , else  pause,end

l=list(1,[1 23],'adssa')
l1=list(123,'sdwqqwq')
if l==0 then pause,end
if 0==l then pause,end
if l==l then,else pause,end
if l<>l then pause,end
if l==l1 then pause,end
if l1==l then pause,end
if l<>l1 then , else pause,end
if l1<>l then , else pause,end


if %t&1==2 then pause,end
if %t|1==2 then, else pause,end
if %t&-1==2 then pause,end
if %t|-1==2 then, else pause,end
if 1<2&1==2 then pause,end
if 1<2|1==2 then, else pause,end
if 1<2&-1==2 then pause,end
if 1<2|-1==2 then, else pause,end
if 2>1&1==2 then pause,end
if 2>1|1==2 then, else pause,end
if 2>1&-1==2 then pause,end
if 2>1|-1==2 then, else pause,end
if 1==1&1==2 then pause,end
if 1==1|1==2 then, else pause,end
if 1==1&-1==2 then pause,end
if 1==1|-1==2 then, else pause,end
if 1<>2&1==2 then pause,end
if 1<>2|1==2 then, else pause,end
if 1<>2&-1==2 then pause,end
if 1<>2|-1==2 then, else pause,end

if %t&1>2 then pause,end
if %t|1>2 then, else pause,end
if %t&-1>2 then pause,end
if %t|-1>2 then, else pause,end
if 1<2&1>2 then pause,end
if 1<2|1>2 then, else pause,end
if 1<2&-1>2 then pause,end
if 1<2|-1>2 then, else pause,end
if 2>1&1>2 then pause,end
if 2>1|1>2 then, else pause,end
if 2>1&-1>2 then pause,end
if 2>1|-1>2 then, else pause,end
if 1==1&1>2 then pause,end
if 1==1|1>2 then, else pause,end
if 1==1&-1>2 then pause,end
if 1==1|-1>2 then, else pause,end
if 1<>2&1>2 then pause,end
if 1<>2|1>2 then, else pause,end
if 1<>2&-1>2 then pause,end
if 1<>2|-1>2 then, else pause,end

