// Copyright INRIA
//extractions
v=int8([1 2 3]);
if v(2)<>int8(2) then pause,end
if or(v([1 3])<>int8([1 3])) then pause,end
if  or(v([3 3])<>int8([3 3])) then pause,end
if  or(v(:)<>int8([1 2 3]')) then pause,end
if v([])<>[] then pause,end
v=int32([1 2 3;4 5 6]);
if v(2)<>int8(4) then pause,end
if  or(v([1 3])<>int8([1;2])) then pause,end
if  or(v([3 3])<>int8([2;2])) then pause,end
if  or(v(:)<>int8([1;4;2;5;3;6])) then pause,end
if v([])<>[] then pause,end

if v(2,1)<>int8(4) then pause,end
if or(v(1,[1 3])<>int8([1 3])) then pause,end
if  or(v(1,[3 3])<>int8([3 3])) then pause,end
if  or(v(1,:)<>int8([1 2 3])) then pause,end
if  or(v(:,2)<>int8([2;5])) then pause,end
if  or(v([1 1],:)<>int8([1 2 3;1 2 3])) then pause,end
if  or(v(:,[2 1])<>int8([2 1;5 4])) then pause,end


if or(v(1,1:$)<>int8([1 2 3])) then pause,end
if or(v(1:$,2)<>int8([2;5])) then pause,end
if or(v([1 1],1:$)<>int8([1 2 3;1 2 3])) then pause,end
if or(v(1:$,[2 1])<>int8([2 1;5 4])) then pause,end




//insertion
v=int8([1 2 3]);
v(2)=int8(3);if or(v<>int8([1 3 3])) then pause,end

v(4)=int8(-5);if or(v<>int8([1 3 3 -5])) then pause,end

v([1 4])=int8([2 5]);if or(v<>int8([2 3 3 5])) then pause,end
v([1 1])=int8([2 5]);if or(v<>int8([5 3 3 5])) then pause,end
v([1 1 1 1 1 1])=int8([2 5 6 7 8 9]);if or(v<>int8([9 3 3 5])) then pause,end
v([4 2])=int8([8 5]);if or(v<>int8([9 5 3 8])) then pause,end

v=uint16([1 2 3]);
v(2)=uint16(3);if or(v<>uint16([1 3 3])) then pause,end

v(4)=uint16(-5);if or(v<>uint16([1 3 3 -5])) then pause,end

v([1 4])=uint16([2 5]);if or(v<>uint16([2 3 3 5])) then pause,end
v([1 1])=uint16([2 5]);if or(v<>uint16([5 3 3 5])) then pause,end
v([1 1 1 1 1 1])=uint16([2 5 6 7 8 9]);if or(v<>uint16([9 3 3 5])) then pause,end
v([4 2])=uint16([8 5]);if or(v<>uint16([9 5 3 8])) then pause,end



v=uint8([1 2 3]);
v(2,4)=uint8(10);if or(v<>uint8([1 2 3 0;0 0 0 10])) then pause,end
v([2 2],4)=uint8([10;20]);if or(v<>uint8([1 2 3 0;0 0 0 20])) then pause,end
v([2 1],4)=uint8([10;20]);if or(v<>uint8([1 2 3 20;0 0 0 10])) then pause,end
v([2 2 2],4)=uint8([10;20;30]);if or(v<>uint8([1 2 3 20;0 0 0 30])) then pause,end

v=int32([1 2 3]);
v(2,4)=int32(10);if or(v<>int32([1 2 3 0;0 0 0 10])) then pause,end
v([2 2],4)=int32([10;20]);if or(v<>int32([1 2 3 0;0 0 0 20])) then pause,end
v([2 1],4)=int32([10;20]);if or(v<>int32([1 2 3 20;0 0 0 10])) then pause,end
v([2 2 2],4)=int32([10;20;30]);if or(v<>int32([1 2 3 20;0 0 0 30])) then pause,end



v=int16([1 2 3;0 0 10]);
v(1,[1 1 1 1])=int16(1:4);;if or(v<>int16([4 2 3;0 0 10])) then pause,end
v([2 2],[1 1 1 1])=int16([1:4;2:5]);if or(v<>int16([4 2 3;5 0 10])) then pause,end
v(3,4)=int16(7);if or(v<>int16([4 2 3 0;5 0 10 0;0 0 0 7])) then pause,end

v=uint32([1 2 3;0 0 10]);
v(1,[1 1 1 1])=uint32(1:4);;if or(v<>uint32([4 2 3;0 0 10])) then pause,end
v([2 2],[1 1 1 1])=uint32([1:4;2:5]);if or(v<>uint32([4 2 3;5 0 10])) then pause,end
v(3,4)=uint32(7);if or(v<>uint32([4 2 3 0;5 0 10 0;0 0 0 7])) then pause,end



v=[];
v(5)=int8(1);if or(v<>int8([0;0;0;0;1])) then pause,end
v(2)=int8(4);if or(v<>int8([0;4;0;0;1])) then pause,end

v=uint16([1 2 3;4 5 6]);
v(:,1)=uint16([4;1]);if or(v<>uint16([4 2 3;1 5 6])) then pause,end
v(:,[1 1])=uint16([4 5;1 2]);if or(v<>uint16([5 2 3;2 5 6])) then pause,end
v(:,[3 1])=uint16([4 5;1 2]);if or(v<>uint16([5 2 4;2 5 1])) then pause,end
v(:,[3 3 3 3])=uint16([4 5 6 7;1 2 3 4]);if or(v<>uint16([5 2 7;2 5 4])) then pause,end

v=int8([1 2 3;4 5 6]);
v(1,:)=int8([2 3 4]);if or(v<>int8([2 3 4;4 5 6])) then pause,end
v([1 1],:)=int8([2 3 4;4 5 6]);if or(v<>int8([4 5 6;4 5 6])) then pause,end
v([2 2 2 2],:)=int8([2 3 4;4 5 6;7 8 9;10 11 12]);
if or(v<>int8([4 5 6;10 11 12])) then pause,end

v=uint8([1 2 3]);
v(2)=[];if or(v<>uint8([1 3])) then pause,end
v=uint8([1 2 3]);
v([3 2])=[];if v<>uint8(1) then pause,end

v=int8([1 2 3]');
v(2)=[];if or(v<>int8([1 3]')) then pause,end
v=int8([1 2 3]');
v([3 2])=[];if v<>int8(1) then pause,end

v=int32([1 2 3;4 5 6]);
v(1,:)=[];if or(v<>int32([4 5 6])) then pause,end
v=int32([1 2 3;4 5 6]);
v(:,[3 2])=[];if or(v<>int32([1;4])) then pause,end

//concatenations

x1=int8(1);x2=int8(7);
if or([x1 x2]<>int8([1 7])) then pause,end
if or([x1;x2]<>int8([1;7])) then pause,end
if or([x1 []]<>int8([1])) then pause,end
if or([x1;[]]<>int8([1])) then pause,end
if or([[] x2]<>int8([7])) then pause,end
if or([[];x2]<>int8([7])) then pause,end
[x1,x2]=([x1 x2 x1 x2],[x2 x2 x1 x1]);

if or([x1 x2]<>int8([1 7 1 7 7 7 1 1])) then pause,end
if or([x1;x2]<>int8([1 7 1 7;7 7 1 1])) then pause,end
if or([x1 []]<>int8([1 7 1 7])) then pause,end
if or([x1;[]]<>int8([1 7 1 7])) then pause,end
if or([[] x2]<>int8([7 7 1 1])) then pause,end
if or([[];x2]<>int8([7 7 1 1])) then pause,end

n=300;x1=uint16(1:n);
if or([x1 x1]<>uint16([1:n 1:n])) then pause,end
if or([x1;x1]<>uint16([1:n;1:n])) then pause,end

//additions soustractions
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
if or(x1+x2<>int8(X1+X2)) then pause,end
if or(x1-x2<>int8(X1-X2)) then pause,end
X1=1:10;x1=int8(X1);
if or(x1+x2<>int8(X1+X2)) then pause,end
if or(x1-x2<>int8(X1-X2)) then pause,end
X2=2:11;x2=int8(X2);
if or(x1+x2<>int8(X1+X2)) then pause,end
if or(x1-x2<>int8(X1-X2)) then pause,end
if or(-x1<>int8(-(1:10))) then pause,end

X2=[];x2=[]
if or(x1+x2<>int8(X1+X2)) then pause,end
if or(x1-x2<>int8(X1-X2)) then pause,end
if or(x2+x1<>int8(X2+X1)) then pause,end
if or(x2-x1<>int8(X2-X1)) then pause,end
//multiplication
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
if or(x1*x2<>int8(X1*X2)) then pause,end
if or(x1.*x2<>int8(X1.*X2)) then pause,end
X1=1:10;x1=int8(X1);
if or(x1*x2<>int8(X1*X2)) then pause,end
if or(x2*x1<>int8(X2*X1)) then pause,end

if or(x1.*x2<>int8(X1.*X2)) then pause,end
if or(x2.*x1<>int8(X2.*X1)) then pause,end
X2=-(1:10);x2=int8(X2);
if or(x1.*x2<>int8(X1.*X2)) then pause,end
if or(x1'*x2<>int8(X1'*X2)) then pause,end
if or(x1*x2'<>int8(X1*X2')) then pause,end

// comparaisons
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
if or((x1>x2)<>(X1>X2)) then pause,end
if or((x1>=x2)<>(X1>=X2)) then pause,end
if or((x1<x2)<>(X1<X2)) then pause,end
if or((x1<=x2)<>(X1<=X2)) then pause,end
if or((x1<>x2)<>(X1<>X2)) then pause,end
X1=1:10;x1=int8(X1);
if or((x1>x2)<>(X1>X2)) then pause,end
if or((x1>=x2)<>(X1>=X2)) then pause,end
if or((x1<x2)<>(X1<X2)) then pause,end
if or((x1<=x2)<>(X1<=X2)) then pause,end
if or((x1<>x2)<>(X1<>X2)) then pause,end

X2=2:11;x2=int8(X2);
if or((x1>x2)<>(X1>X2)) then pause,end
if or((x1>=x2)<>(X1>=X2)) then pause,end
if or((x1<x2)<>(X1<X2)) then pause,end
if or((x1<=x2)<>(X1<=X2)) then pause,end
if or((x1<>x2)<>(X1<>X2)) then pause,end

X1=1;x1=int8(X1);
if or((x1>x2)<>(X1>X2)) then pause,end
if or((x1>=x2)<>(X1>=X2)) then pause,end
if or((x1<x2)<>(X1<X2)) then pause,end
if or((x1<=x2)<>(X1<=X2)) then pause,end
if or((x1<>x2)<>(X1<>X2)) then pause,end

//division
X1=1;X2=7;x1=int8(X1);x2=int8(X2);
if or(x1/x2<>int8(0)) then pause,end
if or(x2/x1<>int8(7)) then pause,end
if or(x1\x2<>int8(7)) then pause,end
if or(x2\x1<>int8(0)) then pause,end
if or(x1./x2<>int8(0)) then pause,end
if or(x2./x1<>int8(7)) then pause,end
if or(x1.\x2<>int8(7)) then pause,end
if or(x2.\x1<>int8(0)) then pause,end

X1=1:10;x1=int8(X1);
if or(x1/x2<>int8([0 0 0 0 0 0 1 1 1 1])) then pause,end
//if or(x2/x1<>int8([7 3 2 1 1 1 1 0 0 0])) then pause,end
if or(x1./x2<>int8([0 0 0 0 0 0 1 1 1 1])) then pause,end
if or(x2./x1<>int8([7 3 2 1 1 1 1 0 0 0])) then pause,end
//if or(x1\x2<>int8([0 0 0 0 0 0 1 1 1 1])) then pause,end
if or(x1.\x2<>int8([7 3 2 1 1 1 1 0 0 0])) then pause,end
if or(x2\x1<> int8([0 0 0 0 0 0 1 1 1 1])) then pause,end
if or(x2.\x1<>int8([0 0 0 0 0 0 1 1 1 1])) then pause,end

X2=-(1:10);x2=int8(X2);
if or(x1./x2<>int8(-ones(1,10))) then pause,end
if or(x1.\x2<>int8(-ones(1,10))) then pause,end
// puissance


X1=2;X2=3;x1=int8(X1);x2=int8(X2);
if or(x1^x2<>int8(X1^X2)) then pause,end
if or(x1.^x2<>int8(X1.^X2)) then pause,end
X1=1:5;x1=int8(X1);
if or(x1^x2<>int8(X1^X2)) then pause,end
if or(x1.^x2<>int8(X1.^X2)) then pause,end
X1=2;x1=int8(X1);X2=(1:3);x2=int8(X2);
if or(x1^x2<>int8(X1^X2)) then pause,end
if or(x1.^x2<>int8(X1.^X2)) then pause,end
X1=2:4;x1=int8(X1);
//if or(x1.^x2<>int8(X1.^X2)) then pause,end

//kronecker
X1=2;X2=3;x1=int8(X1);x2=int8(X2);
if or(x1.*.x2<>int8(X1.*.X2)) then pause,end
X1=1:5;x1=int8(X1);
if or(x1.*.x2<>int8(X1.*.X2)) then pause,end
X1=2;x1=int8(X1);X2=(1:3);x2=int8(X2);
if or(x1.*.x2<>int8(X1.*.X2)) then pause,end
X1=(1:5)';x1=int8(X1);
if or(x1.*.x2<>int8(X1.*.X2)) then pause,end

//sum
X=round(10*rand(1,30));
deff('y=INT(x)','y=int16(x)')
x=INT(X);
if or(sum(x)<>INT(sum(X))) then pause,end
if or(sum(x')<>INT(sum(X'))) then pause,end
if or(cumsum(x)<>INT(cumsum(X))) then pause,end
if or(cumsum(x')<>INT(cumsum(X'))) then pause,end
deff('y=INT(x)','y=uint16(x)')
x=INT(X);
if or(sum(x)<>INT(sum(X))) then pause,end
if or(sum(x')<>INT(sum(X'))) then pause,end
if or(cumsum(x)<>INT(cumsum(X))) then pause,end
if or(cumsum(x')<>INT(cumsum(X'))) then pause,end
deff('y=INT(x)','y=uint8(x)')
x=INT(X);
if or(sum(x)<>INT(sum(X))) then pause,end
if or(sum(x')<>INT(sum(X'))) then pause,end
if or(cumsum(x)<>INT(cumsum(X))) then pause,end
if or(cumsum(x')<>INT(cumsum(X'))) then pause,end
deff('y=INT(x)','y=int8(x)')
x=INT(X);
if or(sum(x)<>INT(sum(X))) then pause,end
if or(sum(x')<>INT(sum(X'))) then pause,end
if or(cumsum(x)<>INT(cumsum(X))) then pause,end
if or(cumsum(x')<>INT(cumsum(X'))) then pause,end
X=round(10*rand(20,30));
x=INT(X);
if or(sum(x,1)<>INT(sum(X,1))) then pause,end
if or(sum(x',1)<>INT(sum(X',1))) then pause,end
if or(cumsum(x,1)<>INT(cumsum(X,1))) then pause,end
if or(cumsum(x',1)<>INT(cumsum(X',1))) then pause,end
deff('y=INT(x)','y=uint16(x)')
x=INT(X);
if or(sum(x,1)<>INT(sum(X,1))) then pause,end
if or(sum(x',1)<>INT(sum(X',1))) then pause,end
if or(cumsum(x,1)<>INT(cumsum(X,1))) then pause,end
if or(cumsum(x',1)<>INT(cumsum(X',1))) then pause,end
deff('y=INT(x)','y=uint8(x)')
x=INT(X);
if or(sum(x,1)<>INT(sum(X,1))) then pause,end
if or(sum(x',1)<>INT(sum(X',1))) then pause,end
if or(cumsum(x,1)<>INT(cumsum(X,1))) then pause,end
if or(cumsum(x',1)<>INT(cumsum(X',1))) then pause,end
deff('y=INT(x)','y=int8(x)')
x=INT(X);
if or(sum(x,1)<>INT(sum(X,1))) then pause,end
if or(sum(x',1)<>INT(sum(X',1))) then pause,end
if or(cumsum(x,1)<>INT(cumsum(X,1))) then pause,end
if or(cumsum(x',1)<>INT(cumsum(X',1))) then pause,end

x=INT(X);
if or(sum(x,'c')<>INT(sum(X,'c'))) then pause,end
if or(sum(x','c')<>INT(sum(X','c'))) then pause,end
if or(cumsum(x,'c')<>INT(cumsum(X,'c'))) then pause,end
if or(cumsum(x','c')<>INT(cumsum(X','c'))) then pause,end
deff('y=INT(x)','y=uint16(x)')
x=INT(X);
if or(sum(x,'c')<>INT(sum(X,'c'))) then pause,end
if or(sum(x','c')<>INT(sum(X','c'))) then pause,end
if or(cumsum(x,'c')<>INT(cumsum(X,'c'))) then pause,end
if or(cumsum(x','c')<>INT(cumsum(X','c'))) then pause,end
deff('y=INT(x)','y=uint8(x)')
x=INT(X);
if or(sum(x,'c')<>INT(sum(X,'c'))) then pause,end
if or(sum(x','c')<>INT(sum(X','c'))) then pause,end
if or(cumsum(x,'c')<>INT(cumsum(X,'c'))) then pause,end
if or(cumsum(x','c')<>INT(cumsum(X','c'))) then pause,end
deff('y=INT(x)','y=int8(x)')
x=INT(X);
if or(sum(x,'c')<>INT(sum(X,'c'))) then pause,end
if or(sum(x','c')<>INT(sum(X','c'))) then pause,end
if or(cumsum(x,'c')<>INT(cumsum(X,'c'))) then pause,end
if or(cumsum(x','c')<>INT(cumsum(X','c'))) then pause,end

//diag triu tril
deff('y=INT(x)','y=int16(x)')

X=round(10*rand(1,30));
x=INT(X);

if or(diag(x)<>diag(X)) then pause,end
if or(diag(x,2)<>diag(X,2)) then pause,end
if or(diag(x,-2)<>diag(X,-2)) then pause,end

if or(triu(x)<>triu(X)) then pause,end
if or(triu(x,2)<>triu(X,2)) then pause,end
if or(triu(x,-2)<>triu(X,-2)) then pause,end

if or(tril(x)<>tril(X)) then pause,end
if or(tril(x,2)<>tril(X,2)) then pause,end
if or(tril(x,-2)<>tril(X,-2)) then pause,end

X=round(10*rand(2,30));
x=INT(X);

if or(diag(x)<>diag(X)) then pause,end
if or(diag(x,2)<>diag(X,2)) then pause,end
if or(diag(x,-2)<>diag(X,-2)) then pause,end

if or(triu(x)<>triu(X)) then pause,end
if or(triu(x,2)<>triu(X,2)) then pause,end
if or(triu(x,-2)<>triu(X,-2)) then pause,end

if or(tril(x)<>tril(X)) then pause,end
if or(tril(x,2)<>tril(X,2)) then pause,end
if or(tril(x,-2)<>tril(X,-2)) then pause,end

deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30));
x=INT(X);

if or(diag(x)<>diag(X)) then pause,end
if or(diag(x,2)<>diag(X,2)) then pause,end
if or(diag(x,-2)<>diag(X,-2)) then pause,end

if or(triu(x)<>triu(X)) then pause,end
if or(triu(x,2)<>triu(X,2)) then pause,end
if or(triu(x,-2)<>triu(X,-2)) then pause,end

if or(tril(x)<>tril(X)) then pause,end
if or(tril(x,2)<>tril(X,2)) then pause,end
if or(tril(x,-2)<>tril(X,-2)) then pause,end

X=round(10*rand(2,30));
x=INT(X);

if or(diag(x)<>diag(X)) then pause,end
if or(diag(x,2)<>diag(X,2)) then pause,end
if or(diag(x,-2)<>diag(X,-2)) then pause,end

if or(triu(x)<>triu(X)) then pause,end
if or(triu(x,2)<>triu(X,2)) then pause,end
if or(triu(x,-2)<>triu(X,-2)) then pause,end

if or(tril(x)<>tril(X)) then pause,end
if or(tril(x,2)<>tril(X,2)) then pause,end
if or(tril(x,-2)<>tril(X,-2)) then pause,end

// save load
deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30));
x=INT(X);

save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');if or(x<>INT(X)) then pause,end

x=list(1,x,2);save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');if or(x<>list(1,INT(X),2)) then pause,end

deff('y=INT(x)','y=int32(x)')
X=round(10*rand(1,30));
x=INT(X);

save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');if or(x<>INT(X)) then pause,end

x=list(1,x,2);save(TMPDIR+'/foo',x)
clear x;load(TMPDIR+'/foo');if or(x<>list(1,INT(X),2)) then pause,end


//abs
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);

if or(abs(x)<>INT(abs(X))) then pause,end

deff('y=INT(x)','y=int16(x)')
X=round(10*rand(1,30))-5;
x=INT(X);

if or(abs(x)<>INT(abs(X))) then pause,end

//matrix
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);

if or(matrix(x,10,-1)<>INT(matrix(X,10,-1))) then pause,end
if or(matrix(x,10,3)<>INT(matrix(X,10,3))) then pause,end
if or(matrix(x,-1,3)<>INT(matrix(X,-1,3))) then pause,end

deff('y=INT(x)','y=uint8(x)')
X=round(10*rand(1,30))-5;
x=INT(X);

if or(matrix(x,10,-1)<>INT(matrix(X,10,-1))) then pause,end
if or(matrix(x,10,3)<>INT(matrix(X,10,3))) then pause,end
if or(matrix(x,-1,3)<>INT(matrix(X,-1,3))) then pause,end

//max min
deff('y=INT(x)','y=int8(x)')
X=round(10*rand(2,30))-5;
Y=ones(2,30);
x=INT(X);
y=INT(Y);
if or(max(x)<>INT(max(X))) then pause,end
if or(max(x,'r')<>INT(max(X,'r'))) then pause,end
if or(max(x,'c')<>INT(max(X,'c'))) then pause,end
if or(max(x,INT(2))<>INT(max(X,2))) then pause,end
if or(max(x,y)<>INT(max(X,Y))) then pause,end

if or(min(x)<>INT(min(X))) then pause,end
if or(min(x,'r')<>INT(min(X,'r'))) then pause,end
if or(min(x,'c')<>INT(min(X,'c'))) then pause,end
if or(min(x,INT(2))<>INT(min(X,2))) then pause,end
if or(min(x,y)<>INT(min(X,Y))) then pause,end


a=[0 3 7];
b=[1 2 7];
A=[0 3 7 5;
   1 2 0 2
   8 9 3 1];
types=['double','int32','uint32','int16','uint16','int8','uint8']

// checking max(a,b)
[mx,kx]=max(a,b);
for t1=types
  for t2=types
    execstr('at='+t1+'(a); bt='+t2+'(b);')
    [mt,kt]=max(at,bt);
    if or(kt<>kx)|or(mx<>double(mt)) then 
      disp('max('+t1+','+t2+')'),pause
    end
  end	
end

// checking min(a,b)
[mn,kn]=min(a,b);
for t1=types
  for t2=types
    execstr('at='+t1+'(a); bt='+t2+'(b);')
    [mt,kt]=min(at,bt);
    if or(kt<>kn)|or(mn<>double(mt)) then 
      disp('min('+t1+','+t2+')'),pause
    end
  end	
end

// checking max(A)
[mx,kx]=max(A)
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At);
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+')'),pause
  end
end

// checking min(A)
[mx,kx]=min(A)
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At);
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+')'),pause
  end
end


// checking max(A,'r')
[mx,kx]=max(A,'r')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At,'r');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+',''r'')'),pause
  end
end

// checking min(A,'r')
[mx,kx]=min(A,'r')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At,'r');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+',''r'')'),pause
  end
end

// checking max(A,'c')
[mx,kx]=max(A,'c')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=max(At,'c');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('max('+t1+',''c'')'),pause
  end
end

// checking min(A,'c')
[mx,kx]=min(A,'c')
for t1=types
  execstr('At='+t1+'(A);')
  [mt,kt]=min(At,'c');
  if or(kt<>kx)|or(mx<>double(mt)) then 
    disp('min('+t1+',''c'')'),pause
  end
end


//int32 int8 ... iconvert
