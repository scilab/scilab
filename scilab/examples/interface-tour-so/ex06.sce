// test with sparse and int matrices 
//------------------------------------

// a sparse as an input-argument 
W=int(10*sprand(10,10,0.1));
[W1,icol,mnel]=ex6c_1(W); 

if norm(full(W1-2*W)) > %eps  then pause,end
[I,J]=find(W<>0) ;
if or(J<>gsort(icol,'g','i')) then pause,end 

mnel1=[];
[m,n]=size(W);
for x=1:m , mnel1(x) = size(find(W(x,:)),'*');end 
if mnel<>mnel1' then pause,end

// a complex sparse as input-argument 

W = W +%i*int(10*sprand(10,10,0.1));

[W1,icol,mnel]=ex6c_1(W); 

if norm(full(W1- (2*real(W)+%i*3*imag(W)))) > %eps  then pause,end
[I,J]=find(W<>(0+%i*0)) ;
if or(J<>gsort(icol,'g','i')) then pause,end 

mnel1=[];
[m,n]=size(W);
for x=1:m , mnel1(x) = size(find(full(W(x,:))<>0),'*');end 
if mnel<>mnel1' then pause,end

// a sparse created in the interface 

W=ex6c_2();
Wres=sparse([2,2,3,3,3,4,5;1,2,1,2,3,3,5]',[1,2,4,5,3,6,7]);
if W<>Wres then pause,end

// an int matrix as argument and as returned value 
//----------------------------------------------

A=int(100*rand(1,10));
[A1,B1,C1]=ex6c_3(int8(A));

if A1<>A then pause,end 
if B1<>int8(A) then pause,end 
if C1<>int8(A) then pause,end 

A=int(100*rand(1,10));
[A1,B1,C1]=ex6c_3(int16(A));

if A1<>A then pause,end 
if B1<>int16(A) then pause,end 
if C1<>int16(A) then pause,end 

if A1<>A then pause,end 
A=int(100*rand(1,10));
[A1,B1,C1]=ex6c_3(int32(A));

if A1<>A then pause,end 
if B1<>int32(A) then pause,end 
if C1<>int32(A) then pause,end 

A1=ex6c_4(A);

if A1<>int16(A) then pause,end 

