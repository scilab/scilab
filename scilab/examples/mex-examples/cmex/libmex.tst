// load the shared library 

exec loader.sce ;

// run tests 

[p,q,r]=mexf01(1:4,'qwerty');
if r~='qwerty' then pause,end

[a,x]=mexf02(20,'x');
if a~=20 then pause,end
A=rand(2,2);B=rand(2,3);

C=mexf03(A,B);
if norm(A*B-C) > %eps then pause,end

p=mexf04(1:3,'x');
if p ~= poly(1:3,'x') then pause,end

w1=mexf05(1:5);
if and(w1~=(1:5)) then pause,end

w2=mexf06(1:5);
if and(w2~=(1:5)) then pause,end
w=rand(2,3);w(10,15)=0;w=sparse(w);

mexf07(mtlb_sparse(w));

w=mexf08();
if w(1)~='123456789 ' then pause,end

mexf09() // prints something calling disp 

clear myvar;

A=mexf10() // search myvar 
if A<>"variable myvar not found" then pause,end
myvar=1:45;
A=mexf10() // search myvar again 
if A<>"variable myvar found size=[1,45]" then pause,end

mexf11() // creates A11 with a mexEvalString
if A11<>[1,2,3,4] then pause,end

mexf12() // creates C with a WriteMatrix (<==> mxPutArray)
if C<>matrix(0:7,4,2) then pause,end

mexf13() // creates X with a mexEvalString 
// then try to get it with mexGetArray 
if X<>[1,2,3] then pause,end


[x,y,z]=mexf14(1,2,3);
if z~=3 then pause,end

[u,v]=mexf15(2,1:3);
if u~=2 then pause,end






