// Copyright INRIA
//test of matelm functions
//size
a=[1 2;3 4];
if or(size(a)<>[2 2]) then pause,end
if or(size(a+0)<>[2 2]) then pause,end
if size(a,'*')<>4 then pause,end
if size(a+0,'*')<>4 then pause,end
[m,n]=size(a);if or([m,n]<>[2 2]) then pause,end

a=[1;2];
if size(a,1)<>2 then pause,end
if size(a+0,1)<>2 then pause,end
if size(a,'r')<>2 then pause,end
if size(a+0,'r')<>2 then pause,end
if size(a,2)<>1 then pause,end
if size(a+0,2)<>1 then pause,end
if size(a,'c')<>1 then pause,end
if size(a+0,'c')<>1 then pause,end
[m,n]=size(a);if or([m,n]<>[2 1]) then pause,end

a=string([1 2;3 4]);
if or(size(a)<>[2 2]) then pause,end
if or(size(a+a)<>[2 2]) then pause,end
if size(a,'*')<>4 then pause,end
if size(a+a,'*')<>4 then pause,end
[m,n]=size(a);if or([m,n]<>[2 2]) then pause,end

a=string([1;2]);
if size(a,1)<>2 then pause,end
if size(a+a,1)<>2 then pause,end
if size(a,'r')<>2 then pause,end
if size(a+a,'r')<>2 then pause,end
if size(a,2)<>1 then pause,end
if size(a+a,2)<>1 then pause,end
if size(a,'c')<>1 then pause,end
if size(a+a,'c')<>1 then pause,end
[m,n]=size(a);if or([m,n]<>[2 1]) then pause,end

a=[1 2;3 4]+%s;
if or(size(a)<>[2 2]) then pause,end
if or(size(a+0)<>[2 2]) then pause,end
if size(a,'*')<>4 then pause,end
if size(a+0,'*')<>4 then pause,end
[m,n]=size(a);if or([m,n]<>[2 2]) then pause,end

a=[1;2]+%s;
if size(a,1)<>2 then pause,end
if size(a+0,1)<>2 then pause,end
if size(a,'r')<>2 then pause,end
if size(a+0,'r')<>2 then pause,end
if size(a,2)<>1 then pause,end
if size(a+0,2)<>1 then pause,end
if size(a,'c')<>1 then pause,end
if size(a+0,'c')<>1 then pause,end
[m,n]=size(a);if or([m,n]<>[2 1]) then pause,end

a=[1 2;3 4]==1;
if or(size(a)<>[2 2]) then pause,end
if or(size(a&a)<>[2 2]) then pause,end
if size(a,'*')<>4 then pause,end
if size(a|a,'*')<>4 then pause,end
[m,n]=size(a);if or([m,n]<>[2 2]) then pause,end

a=[1;2]==1;
if size(a,1)<>2 then pause,end
if size(a|a,1)<>2 then pause,end
if size(a,'r')<>2 then pause,end
if size(a|a,'r')<>2 then pause,end
if size(a,2)<>1 then pause,end
if size(a|a,2)<>1 then pause,end
if size(a,'c')<>1 then pause,end
if size(a|a,'c')<>1 then pause,end
[m,n]=size(a);if or([m,n]<>[2 1]) then pause,end

//eye
if or(eye(2,2)<>[1 0;0 1]) then pause,end
if or(eye(2,1)<>[1;0]) then pause,end
n=2;
if or(eye(n,2)<>[1 0;0 1]) then pause,end
if or(eye(2,n)<>[1 0;0 1]) then pause,end
if or(eye(n,n)<>[1 0;0 1]) then pause,end
if or(size(eye())<>[-1 -1]) then pause,end
a=[1 2;3 4];
if or(eye(a)<>[1 0;0 1]) then pause,end
if or(eye(a+0)<>[1 0;0 1]) then pause,end
a=[1 2;3 4]+%s;
if or(eye(a)<>[1 0;0 1]) then pause,end
if or(eye(a+0)<>[1 0;0 1]) then pause,end
a=string([1 2;3 4]);
if or(eye(a)<>[1 0;0 1]) then pause,end
if or(eye(a+a)<>[1 0;0 1]) then pause,end
a=[1 2;3 4]>1;
if or(eye(a)<>[1 0;0 1]) then pause,end
if or(eye(a&a)<>[1 0;0 1]) then pause,end

if eye([])<>[] then pause,end
//ones
if or(ones(2,2)<>[1 1;1 1]) then pause,end
if or(ones(2,1)<>[1;1]) then pause,end
n=2;
if or(ones(n,2)<>[1 1;1 1]) then pause,end
if or(ones(2,n)<>[1 1;1 1]) then pause,end
if or(ones(n,n)<>[1 1;1 1]) then pause,end
if ones(1)<>1 then pause,end
if ones([])<>[] then pause,end
a=[1 2;3 4];
if or(ones(a)<>[1 1;1 1]) then pause,end
if or(ones(a+0)<>[1 1;1 1]) then pause,end
a=[1 2;3 4]+%s;
if or(ones(a)<>[1 1;1 1]) then pause,end
if or(ones(a+0)<>[1 1;1 1]) then pause,end
a=string([1 2;3 4]);
if or(ones(a)<>[1 1;1 1]) then pause,end
if or(ones(a+a)<>[1 1;1 1]) then pause,end
a=[1 2;3 4]>1;
if or(ones(a)<>[1 1;1 1]) then pause,end
if or(ones(a&a)<>[1 1;1 1]) then pause,end
//rand
if or(size(rand(2,2))<>[2 2]) then pause,end
if or(size(rand(2,1))<>[2 1]) then pause,end
n=2;
if or(size(rand(n,2))<>[n 2]) then pause,end
if or(size(rand(2,n))<>[2 n]) then pause,end
if or(size(rand(n,n))<>[n n]) then pause,end
//if or(size(rand())<>[1 1]) then pause,end
if rand([])<>[] then pause,end
a=[1 2;3 4];
if or(size(rand(a))<>size(a)) then pause,end
if or(size(rand(a+0))<>size(a)) then pause,end
//a=[1 2;3 4]+%s;
//if or(size(rand(a))<>size(a)) then pause,end
//if or(size(rand(a+0))<>size(a)) then pause,end
//a=string([1 2;3 4]);
//if or(size(rand(a))<>size(a)) then pause,end
//if or(size(rand(a+a))<>size(a)) then pause,end
//a=[1 2;3 4]>1;
//if or(size(rand(a))<>size(a)) then pause,end
//if or(size(rand(a+a))<>size(a)) then pause,end
rand('seed',1);if rand('seed')<>1 then pause,end
a=2;rand('seed',a);if rand('seed')<>a then pause,end

//diag
if or(diag([1 2 3])<>[1 0 0;0 2 0;0 0 3]) then pause,end
a=[1 2 3];
if or(diag(a)<>[1 0 0;0 2 0;0 0 3]) then pause,end
if or(diag([1 %i 2])<>[1 0 0;0 %i 0;0 0 2]) then pause,end
a=[1 %i 2 ];
if or(diag(a)<>[1 0 0;0 %i 0;0 0 2]) then pause,end
a=[1 2 3 4;5 6 7 8];
if or(diag(a)<>[1;6]) then pause,end
if or(diag(a+0)<>[1;6]) then pause,end
if or(diag(a,1)<>[2;7]) then pause,end
if or(diag(a+0,1)<>[2;7]) then pause,end
if or(diag(a,-1)<>[5]) then pause,end
if or(diag(a+0,-1)<>[5]) then pause,end
if diag(a,4)<>[] then pause,end
a(1,1)=%i
if or(diag(a)<>[%i;6]) then pause,end
if or(diag(a+0)<>[%i;6]) then pause,end
if or(diag(a,1)<>[2;7]) then pause,end
if or(diag(a+0,1)<>[2;7]) then pause,end
if or(diag(a,-1)<>[5]) then pause,end
if or(diag(a+0,-1)<>[5]) then pause,end
if diag(a,4)<>[] then pause,end

if diag([])<>[] then pause,end
if diag([],1)<>[] then pause,end
if diag([],-1)<>[] then pause,end
a=[];
if diag(a)<>[] then pause,end
if diag(a,1)<>[] then pause,end
if diag(a,-1)<>[] then pause,end

// triu
a=[1 2 3 4;5 6 7 8];
if or(triu(a)<>[1 2 3 4;0 6 7 8]) then pause,end
if or(triu(a+0)<>[1 2 3 4;0 6 7 8]) then pause,end
if or(triu(a,1)<>[0 2 3 4;0 0 7 8]) then pause,end
if or(triu(a+0,1)<>[0 2 3 4;0 0 7 8]) then pause,end
if or(triu(a,-1)<>a) then pause,end
if or(triu(a+0,-1)<>a) then pause,end
if or(triu(a,4)<>0*a) then pause,end
a(1,1)=%i;
if or(triu(a)<>[%i 2 3 4;0 6 7 8]) then pause,end
if or(triu(a+0)<>[%i 2 3 4;0 6 7 8]) then pause,end
if or(triu(a,1)<>[0 2 3 4;0 0 7 8]) then pause,end
if or(triu(a+0,1)<>[0 2 3 4;0 0 7 8]) then pause,end
if or(triu(a,-1)<>a) then pause,end
if or(triu(a+0,-1)<>a) then pause,end
if or(triu(a,4)<>0*a) then pause,end

if triu([])<>[] then pause,end
if triu([],1)<>[] then pause,end
if triu([],-1)<>[] then pause,end
a=[];
if triu(a)<>[] then pause,end
if triu(a,1)<>[] then pause,end
if triu(a,-1)<>[] then pause,end

//tril
a=[1 2 3 4;5 6 7 8];
if or(tril(a)<>[1 0 0 0;5 6 0 0]) then pause,end
if or(tril(a+0)<>[1 0 0 0;5 6 0 0]) then pause,end
if or(tril(a,1)<>[1 2 0 0;5 6  7 0]) then pause,end
if or(tril(a+0,1)<>[1 2 0 0;5 6  7 0]) then pause,end
if or(tril(a,4)<>a) then pause,end
if or(tril(a+0,4)<>a) then pause,end
if or(tril(a,-3)<>0*a) then pause,end
a(1,1)=%i;
if or(tril(a)<>[%i 0 0 0;5 6 0 0]) then pause,end
if or(tril(a+0)<>[%i 0 0 0;5 6 0 0]) then pause,end
if or(tril(a,1)<>[%i 2 0 0;5 6  7 0]) then pause,end
if or(tril(a+0,1)<>[%i 2 0 0;5 6  7 0]) then pause,end
if or(tril(a,4)<>a) then pause,end
if or(tril(a+0,4)<>a) then pause,end
if or(tril(a,-3)<>0*a) then pause,end

if tril([])<>[] then pause,end
if tril([],1)<>[] then pause,end
if tril([],-1)<>[] then pause,end
a=[];
if tril(a)<>[] then pause,end
if tril(a,1)<>[] then pause,end
if tril(a,-1)<>[] then pause,end


//abs
a=[1 2;-3 4;5 -6];
if or(abs(a)<>[1 2;3 4;5 6]) then pause,end
if or(abs(a+0)<>[1 2;3 4;5 6]) then pause,end
if abs([])<>[] then pause,end
a=[];if abs(a)<>[] then pause,end
a=[1+%i 2;-3 4;5 -6];
if or(abs(a)<>[sqrt(2) 2;3 4;5 6]) then pause,end
if or(abs(a+0)<>[sqrt(2) 2;3 4;5 6]) then pause,end

a=[1 2;-3 4;5 -6]+%s;
if or(abs(a)<>[1 2;3 4;5 6]+%s) then pause,end
if or(abs(a+0)<>[1 2;3 4;5 6]+%s) then pause,end
a=[1+%i 2;-3 4;5 -6]+%s;
if or(abs(a)<>[sqrt(2) 2;3 4;5 6]+%s) then pause,end
if or(abs(a+0)<>[sqrt(2) 2;3 4;5 6]+%s) then pause,end

a=sparse([1 2;-3 4;5 -6]);
if or(full(abs(a))<>[1 2;3 4;5 6]) then pause,end
if or(full(abs(a+a))<>2*[1 2;3 4;5 6]) then pause,end
a=sparse([1+%i 2;-3 4;5 -6]);
if or(full(abs(a))<>[sqrt(2) 2;3 4;5 6]) then pause,end
if or(full(abs(a+a))<>2*[sqrt(2) 2;3 4;5 6]) then pause,end

//real
a=[1 2;-3 4;5 -6];
if or(real(a)<>a) then pause,end
if or(real(a+0)<>a) then pause,end
if real([])<>[] then pause,end
a=[];if real(a)<>[] then pause,end
a=[1+%i 2;-3 4;5 -6];
if or(real(a)<>[1 2;-3 4;5 -6]) then pause,end
if or(real(a+0)<>[1 2;-3 4;5 -6]) then pause,end

a=[1 2;-3 4;5 -6]+%s;
if or(real(a)<>a) then pause,end
if or(real(a+0)<>a) then pause,end
a=[1+%i 2;-3 4;5 -6]+%s;
if or(real(a)<>[1 2;-3 4;5 -6]+%s) then pause,end
if or(real(a+0)<>[1 2;-3 4;5 -6]+%s) then pause,end

//a=sparse([1 2;-3 4;5 -6]);
//if or(real(a)<>a) then pause,end
//if or(real(a+a)<>2*a) then pause,end
//a=sparse([1+%i 2;-3 4;5 -6]);
//if or(full(real(a))<>[1 2;-3 4;5 -6]) then pause,end
//if or(full(real(a+a))<>2*[1 2;-3 4;5 -6]) then pause,end

//imag
a=[1 2;-3 4;5 -6];
if or(imag(a)<>0*a) then pause,end
if or(imag(a+0)<>0*a) then pause,end
if imag([])<>[] then pause,end
a=[];if imag(a)<>[] then pause,end
a=[1+%i 2;-3 4;5 -6];
if or(imag(a)<>[1 0;0 0;0 0]) then pause,end
if or(imag(a+0)<>[1 0;0 0;0 0]) then pause,end

a=[1 2;-3 4;5 -6]+%s;
if or(imag(a)<>0*a) then pause,end
if or(imag(a+0)<>0*a) then pause,end
a=[1+%i 2;-3 4;5 -6]+%s;
if or(imag(a)<>[poly(1,'s','c') 0;0 0;0 0]) then pause,end
if or(imag(a+0)<>[poly(1,'s','c') 0;0 0;0 0]) then pause,end

//a=sparse([1 2;-3 4;5 -6]);
//if or(imag(a)<>0*a) then pause,end
//if or(imag(a+a)<>0*a) then pause,end
//a=sparse([1+%i 2;-3 4;5 -6]);
//if or(full(imag(a))<>[1 0;0 0;0 0]) then pause,end
//if or(full(imag(a+a))<>2*[1 0;0 0;0 0]) then pause,end


//conj
a=[1 2;-3 4;5 -6];
if or(conj(a)<>a) then pause,end
if or(conj(a+0)<>a) then pause,end
if conj([])<>[] then pause,end
a=[];if conj(a)<>[] then pause,end
a=[1+%i 2;-3 4;5 -6];
if or(conj(a)<>[1-%i 2;-3 4;5 -6]) then pause,end
if or(conj(a+0)<>[1-%i 2;-3 4;5 -6]) then pause,end

a=[1 2;-3 4;5 -6]+%s;
if or(conj(a)<>a) then pause,end
if or(conj(a+0)<>a) then pause,end
a=[1+%i 2;-3 4;5 -6]+%s;
if or(conj(a)<>[1-%i 2;-3 4;5 -6]+%s) then pause,end
if or(conj(a+0)<>[1-%i 2;-3 4;5 -6]+%s) then pause,end

//a=sparse([1 2;-3 4;5 -6]);
//if or(conj(a)<>a) then pause,end
//if or(conj(a+a)<>2*a) then pause,end
//a=sparse([1+%i 2;-3 4;5 -6]);
//if or(full(conj(a))<>[1-%i 2;-3 4;5 -6]) then pause,end
//if or(full(conj(a+a))<>2*[1-%i 2;-3 4;5 -6]) then pause,end




//int
a=[1.2 2.5;-3.4 4.5;5.8 -6.2];
if or(int(a)<>[1 2;-3 4;5 -6]) then pause,end
if or(int(a+0)<>[1 2;-3 4;5 -6]) then pause,end
a=[1.2+2.5*%i 2.5;-3.4 4.5;5.8 -6.2];
if or(int(a)<>[1+2*%i 2;-3 4;5 -6]) then pause,end
if or(int(a+0)<>[1+2*%i 2;-3 4;5 -6]) then pause,end

a=[1.2 2.5;-3.4 4.5;5.8 -6.2]+1.1*%s;
if or(int(a)<>[1 2;-3 4;5 -6]+%s) then pause,end
if or(int(a+0)<>[1 2;-3 4;5 -6]+%s) then pause,end
a=[1.2+2.5*%i 2.5;-3.4 4.5;5.8 -6.2]+1.1*%s;
if or(int(a)<>[1+2*%i 2;-3 4;5 -6]+%s) then pause,end
if or(int(a+0)<>[1+2*%i 2;-3 4;5 -6]+%s) then pause,end

if int([])<>[] then pause,end
a=[];if int(a)<>[] then pause,end


//round

a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if or(round(a)<>[1 3;-3 5;6 -6]) then pause,end
if or(round(a+0)<>[1 3;-3 5;6 -6]) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2];
if or(round(a)<>[1+3*%i 3;-3 5;6 -6]) then pause,end
if or(round(a+0)<>[1+3*%i 3;-3 5;6 -6]) then pause,end

a=[1.2 2.51;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(round(a)<>[1 3;-3 5;6 -6]+%s) then pause,end
if or(round(a+0)<>[1 3;-3 5;6 -6]+%s) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(round(a)<>[1+3*%i 3;-3 5;6 -6]+%s) then pause,end
if or(round(a+0)<>[1+3*%i 3;-3 5;6 -6]+%s) then pause,end

if round([])<>[] then pause,end
a=[];if round(a)<>[] then pause,end

//ceil
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if or(ceil(a)<>[2 3;-3 5;6 -6]) then pause,end
if or(ceil(a+0)<>[2 3;-3 5;6 -6]) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2];
if or(ceil(a)<>[2+3*%i 3;-3 5;6 -6]) then pause,end
if or(ceil(a+0)<>[2+3*%i 3;-3 5;6 -6]) then pause,end

a=[1.2 2.51;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(ceil(a)<>[2 3;-3 5;6 -6]+2*%s) then pause,end
if or(ceil(a+0)<>[2 3;-3 5;6 -6]+2*%s) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(ceil(a)<>[2+3*%i 3;-3 5;6 -6]+2*%s) then pause,end
if or(ceil(a+0)<>[2+3*%i 3;-3 5;6 -6]+2*%s) then pause,end

if ceil([])<>[] then pause,end
a=[];if ceil(a)<>[] then pause,end


//floor
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if or(floor(a)<>[1 2;-4 4;5 -7]) then pause,end
if or(floor(a+0)<>[1 2;-4 4;5 -7]) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2];
if or(floor(a)<>[1+2*%i 2;-4 4;5 -7]) then pause,end
if or(floor(a+0)<>[1+2*%i 2;-4 4;5 -7]) then pause,end

a=[1.2 2.51;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(floor(a)<>[1 2;-4 4;5 -7]+%s) then pause,end
if or(floor(a+0)<>[1 2;-4 4;5 -7]+%s) then pause,end
a=[1.2+2.51*%i 2.52;-3.4 4.52;5.8 -6.2]+1.1*%s;
if or(floor(a)<>[1+2*%i 2;-4 4;5 -7]+%s) then pause,end
if or(floor(a+0)<>[1+2*%i 2;-4 4;5 -7]+%s) then pause,end

if floor([])<>[] then pause,end
a=[];if floor(a)<>[] then pause,end

//sign
//a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
//if or(sign(a)<>[1 1;-1 1;1 -1]) then pause,end
//a=[1+%i 2.51;-3.4 4.52;5.8 -6.2];
//if norm(sign(a)-[sqrt(2)/2*(1+%i) 1;-1 1;1 -1])>10*%eps then pause,end
//if sign([])<>[] then pause,end
//a=[];if sign(a)<>[] then pause,end


//clean

//log and exp
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if norm(exp(log(a))-a)>10*%eps then pause,end
if norm(exp(log(a+0))-a)>10*%eps then pause,end
b=log(a);if norm(exp(b)-a)>10*%eps then pause,end
a=[1+%i 2.51;-3.4 4.52;5.8 -6.2];
if norm(exp(log(a))-a)>10*%eps then pause,end
if norm(exp(log(a+0))-a)>10*%eps then pause,end
b=log(a);if norm(exp(b)-a)>10*%eps then pause,end

if exp([])<>[] then pause,end
a=[];if exp(a)<>[] then pause,end
if log([])<>[] then pause,end
a=[];if log(a)<>[] then pause,end

//sin and cos
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if norm(sin(a).^2+cos(a).^2-1)>10*%eps then pause,end
if norm(sin(a+0).^2+cos(a+0).^2-1)>10*%eps then pause,end
a=[1+%i 2.51;-3.4 4.52;5.8 -6.2];
if norm(sin(a).^2+cos(a).^2-1)>10*%eps then pause,end
if norm(sin(a+0).^2+cos(a+0).^2-1)>10*%eps then pause,end

if cos([])<>[] then pause,end
a=[];if cos(a)<>[] then pause,end
if sin([])<>[] then pause,end
a=[];if sin(a)<>[] then pause,end


//tan et atan
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if norm(tan(atan(a))-a)>100*%eps then pause,end
if norm(tan(atan(a+0))-a)>100*%eps then pause,end
b=log(a);if norm(exp(b)-a)>100*%eps then pause,end
a=[1+%i 2.51;-3.4 4.52;5.8 -6.2];
if norm(tan(atan(a))-a)>100*%eps then pause,end
if norm(tan(atan(a+0))-a)>100*%eps then pause,end

if atan([])<>[] then pause,end
a=[];if atan(a)<>[] then pause,end

if atan([],[])<>[] then pause,end
a=[];if atan(a,[])<>[] then pause,end
a=[];if atan(a,a)<>[] then pause,end
a=[];if atan([],a)<>[] then pause,end

//expm
a=[0 2;0 0];
if norm(expm(a)-[1 2;0 1])>10*%eps then pause,end
a=[0 2*%i;0 0];
if norm(expm(a)-[1 2*%i;0 1])>10*%eps then pause,end
if expm([])<>[] then pause,end
a=[];if expm(a)<>[] then pause,end

//sqrt
a=[1.2 2.51;-3.4 4.52;5.8 -6.2];
if norm(sqrt(a).^2-a)>100*%eps then pause,end
if norm(sqrt(a+0).^2-a)>100*%eps then pause,end
a=[1+%i 2.51;-3.4 4.52;5.8 -6.2];
if norm(sqrt(a).^2-a)>100*%eps then pause,end
if norm(sqrt(a+0).^2-a)>100*%eps then pause,end

if sqrt([])<>[] then pause,end
a=[];if sqrt(a)<>[] then pause,end

//sum
a=[1 2;-3 4;5 -6];
if sum(a)<>3 then pause,end
if sum(a+0)<>3 then pause,end
if or(sum(a,1)<>[3 0]) then pause,end
if or(sum(a+0,1)<>[3 0]) then pause,end
n=1;
if or(sum(a,n)<>[3 0]) then pause,end
if or(sum(a+0,n)<>[3 0]) then pause,end
if or(sum(a,'r')<>[3 0]) then pause,end
if or(sum(a+0,'r')<>[3 0]) then pause,end
n='r';
if or(sum(a,n)<>[3 0]) then pause,end
if or(sum(a+0,n)<>[3 0]) then pause,end

if or(sum(a,2)<>[3;1;-1]) then pause,end
if or(sum(a+0,2)<>[3;1;-1]) then pause,end
n=2;
if or(sum(a,n)<>[3;1;-1]) then pause,end
if or(sum(a+0,n)<>[3;1;-1]) then pause,end
if or(sum(a,'c')<>[3;1;-1]) then pause,end
if or(sum(a+0,'c')<>[3;1;-1]) then pause,end
n='c';
if or(sum(a,n)<>[3;1;-1]) then pause,end
if or(sum(a+0,n)<>[3;1;-1]) then pause,end

a=[1 2;-3 4;5 -6]+0*%i;
if or(sum(a)<>3+0*%i) then pause,end
if or(sum(a+0)<>3+0*%i) then pause,end
if or(sum(a,1)<>[3 0]+0*%i) then pause,end
if or(sum(a+0,1)<>[3 0]+0*%i) then pause,end
n=1;
if or(sum(a,n)<>[3 0]+0*%i) then pause,end
if or(sum(a+0,n)<>[3 0]+0*%i) then pause,end
if or(sum(a,'r')<>[3 0]+0*%i) then pause,end
if or(sum(a+0,'r')<>[3 0]+0*%i) then pause,end
n='r';
if or(sum(a,n)<>[3 0]+0*%i) then pause,end
if or(sum(a+0,n)<>[3 0]+0*%i) then pause,end

if or(sum(a,2)<>[3;1;-1]+0*%i) then pause,end
if or(sum(a+0,2)<>[3;1;-1]+0*%i) then pause,end
n=2;
if or(sum(a,n)<>[3;1;-1]+0*%i) then pause,end
if or(sum(a+0,n)<>[3;1;-1]+0*%i) then pause,end
if or(sum(a,'c')<>[3;1;-1]+0*%i) then pause,end
if or(sum(a+0,'c')<>[3;1;-1]+0*%i) then pause,end
n='c';
if or(sum(a,n)<>[3;1;-1]+0*%i) then pause,end
if or(sum(a+0,n)<>[3;1;-1]+0*%i) then pause,end

a=[]
if sum(a)<>0 then pause,end
if sum([])<>0 then pause,end
if sum(a,1)<>[] then pause,end
if sum([],1)<>[] then pause,end
n=1;
if sum(a,n)<>[] then pause,end
if sum([],n)<>[] then pause,end
if sum(a,'r')<>[] then pause,end
if sum([],'r')<>[] then pause,end
n='r';
if sum(a,n)<>[] then pause,end
if sum([],n)<>[] then pause,end

if sum(a,2)<>[] then pause,end
if sum([],2)<>[] then pause,end
n=2;
if sum(a,n)<>[] then pause,end
if sum([],n)<>[] then pause,end
if sum(a,'c')<>[] then pause,end
if sum([],'c')<>[] then pause,end
n='c';
if sum(a,n)<>[] then pause,end
if sum([],n)<>[] then pause,end


a=sparse([1 2;-3 4;5 -6]);
if sum(a)<>3 then pause,end
if sum(a+0*a)<>3 then pause,end
a=sparse([1 2;-3 4;5 -6]+0*%i);
if sum(a)<>3+0*%i then pause,end
if sum(a+0*a)<>3+0*%i then pause,end


//prod
a=[1 2;-3 4;5 -6];
if prod(a)<>720 then pause,end
if prod(a+0)<>720 then pause,end
if prod(a,1)<>[-15 -48] then pause,end
if or(prod(a+0,1)<>[-15 -48]) then pause,end
n=1;
if or(prod(a,n)<>[-15 -48]) then pause,end
if or(prod(a+0,n)<>[-15 -48]) then pause,end
if or(prod(a,'r')<>[-15 -48]) then pause,end
if or(prod(a+0,'r')<>[-15 -48]) then pause,end
n='r';
if or(prod(a,n)<>[-15 -48]) then pause,end
if or(prod(a+0,n)<>[-15 -48]) then pause,end

if or(prod(a,2)<>[2;-12;-30]) then pause,end
if or(prod(a+0,2)<>[2;-12;-30]) then pause,end
n=2;
if or(prod(a,n)<>[2;-12;-30]) then pause,end
if or(prod(a+0,n)<>[2;-12;-30]) then pause,end
if or(prod(a,'c')<>[2;-12;-30]) then pause,end
if or(prod(a+0,'c')<>[2;-12;-30]) then pause,end
n='c';
if or(prod(a,n)<>[2;-12;-30]) then pause,end
if or(prod(a+0,n)<>[2;-12;-30]) then pause,end

a=[1 2;-3 4;5 -6]+0*%i;
if prod(a)<>720+0*%i then pause,end
if prod(a+0)<>720+0*%i then pause,end
if or(prod(a,1)<>[-15 -48]+0*%i) then pause,end
if or(prod(a+0,1)<>[-15 -48]+0*%i) then pause,end
n=1;
if or(prod(a,n)<>[-15 -48]+0*%i) then pause,end
if or(prod(a+0,n)<>[-15 -48]+0*%i) then pause,end
if or(prod(a,'r')<>[-15 -48]+0*%i) then pause,end
if or(prod(a+0,'r')<>[-15 -48]+0*%i) then pause,end
n='r';
if or(prod(a,n)<>[-15 -48]+0*%i) then pause,end
if or(prod(a+0,n)<>[-15 -48]+0*%i) then pause,end

if or(prod(a,2)<>[2;-12;-30]+0*%i) then pause,end
if or(prod(a+0,2)<>[2;-12;-30]+0*%i) then pause,end
n=2;
if or(prod(a,n)<>[2;-12;-30]+0*%i) then pause,end
if or(prod(a+0,n)<>[2;-12;-30]+0*%i) then pause,end
if or(prod(a,'c')<>[2;-12;-30]+0*%i) then pause,end
if or(prod(a+0,'c')<>[2;-12;-30]+0*%i) then pause,end
n='c';
if or(prod(a,n)<>[2;-12;-30]+0*%i) then pause,end
if or(prod(a+0,n)<>[2;-12;-30]+0*%i) then pause,end

a=[]
if prod(a)<>1 then pause,end
if prod([])<>1 then pause,end
if prod(a,1)<>[] then pause,end
if prod([],1)<>[] then pause,end
n=1;
if prod(a,n)<>[] then pause,end
if prod([],n)<>[] then pause,end
if prod(a,'r')<>[] then pause,end
if prod([],'r')<>[] then pause,end
n='r';
if prod(a,n)<>[] then pause,end
if prod([],n)<>[] then pause,end

if prod(a,2)<>[] then pause,end
if prod([],2)<>[] then pause,end
n=2;
if prod(a,n)<>[] then pause,end
if prod([],n)<>[] then pause,end
if prod(a,'c')<>[] then pause,end
if prod([],'c')<>[] then pause,end
n='c';
if prod(a,n)<>[] then pause,end
if prod([],n)<>[] then pause,end

//cumsum
a=[1 2;-3 4;5 -6];
if or(cumsum(a)<>[1,5;-2,9;3,3]) then pause,end
if or(cumsum(a+0)<> [1,5;-2,9;3,3]) then pause,end
if or(cumsum(a,1)<>[1,2;-2,6;3 0]) then pause,end
if or(cumsum(a+0,1)<>[1,2;-2,6;3 0]) then pause,end
n=1;
if or(cumsum(a,n)<>[1,2;-2,6;3 0]) then pause,end
if or(cumsum(a+0,n)<>[1,2;-2,6;3 0]) then pause,end
if or(cumsum(a,'r')<>[1,2;-2,6;3 0]) then pause,end
if or(cumsum(a+0,'r')<>[1,2;-2,6;3 0]) then pause,end
n='r';
if or(cumsum(a,n)<>[1,2;-2,6;3 0]) then pause,end
if or(cumsum(a+0,n)<>[1,2;-2,6;3 0]) then pause,end

if or(cumsum(a,2)<>[1,3;-3,1;5,-1]) then pause,end
if or(cumsum(a+0,2)<>[1,3;-3,1;5,-1]) then pause,end
n=2;
if or(cumsum(a,n)<>[1,3;-3,1;5,-1]) then pause,end
if or(cumsum(a+0,n)<>[1,3;-3,1;5,-1]) then pause,end
if or(cumsum(a,'c')<>[1,3;-3,1;5,-1]) then pause,end
if or(cumsum(a+0,'c')<>[1,3;-3,1;5,-1]) then pause,end
n='c';
if or(cumsum(a,n)<>[1,3;-3,1;5,-1]) then pause,end
if or(cumsum(a+0,n)<>[1,3;-3,1;5,-1]) then pause,end

a=[1 2;-3 4;5 -6]+0*%i;
if cumsum(a)<>3+0*%i then pause,end
if cumsum(a+0)<>3+0*%i then pause,end
if or(cumsum(a,1)<>[1,2;-2,6;3 0]+0*%i) then pause,end
if or(cumsum(a+0,1)<>[1,2;-2,6;3 0]+0*%i) then pause,end
n=1;
if or(cumsum(a,n)<>[1,2;-2,6;3 0]+0*%i) then pause,end
if or(cumsum(a+0,n)<>[1,2;-2,6;3 0]+0*%i) then pause,end
if or(cumsum(a,'r')<>[1,2;-2,6;3 0]+0*%i) then pause,end
if or(cumsum(a+0,'r')<>[1,2;-2,6;3 0]+0*%i) then pause,end
n='r';
if or(cumsum(a,n)<>[1,2;-2,6;3 0]+0*%i) then pause,end
if or(cumsum(a+0,n)<>[1,2;-2,6;3 0]+0*%i) then pause,end

if or(cumsum(a,2)<>[1,3;-3,1;5,-1]+0*%i) then pause,end
if or(cumsum(a+0,2)<>[1,3;-3,1;5,-1]+0*%i) then pause,end
n=2;
if or(cumsum(a,n)<>[1,3;-3,1;5,-1]+0*%i) then pause,end
if or(cumsum(a+0,n)<>[1,3;-3,1;5,-1]+0*%i) then pause,end
if or(cumsum(a,'c')<>[1,3;-3,1;5,-1]+0*%i) then pause,end
if or(cumsum(a+0,'c')<>[1,3;-3,1;5,-1]+0*%i) then pause,end
n='c';
if or(cumsum(a,n)<>[1,3;-3,1;5,-1]+0*%i) then pause,end
if or(cumsum(a+0,n)<>[1,3;-3,1;5,-1]+0*%i) then pause,end

a=[];
if cumsum(a)<>[] then pause,end
if cumsum([])<>[] then pause,end
if cumsum(a,1)<>[] then pause,end
if cumsum([],1)<>[] then pause,end
n=1;
if cumsum(a,n)<>[] then pause,end
if cumsum([],n)<>[] then pause,end
if cumsum(a,'r')<>[] then pause,end
if cumsum([],'r')<>[] then pause,end
n='r';
if cumsum(a,n)<>[] then pause,end
if cumsum([],n)<>[] then pause,end

if cumsum(a,2)<>[] then pause,end
if cumsum([],2)<>[] then pause,end
n=2;
if cumsum(a,n)<>[] then pause,end
if cumsum([],n)<>[] then pause,end
if cumsum(a,'c')<>[] then pause,end
if cumsum([],'c')<>[] then pause,end
n='c';
if cumsum(a,n)<>[] then pause,end
if cumsum([],n)<>[] then pause,end


a=sparse([1 2;-3 4;5 -6]);
//if cumsum(a)<> [1,5;-2,9;3,3] then pause,end
//if cumsum(a+0*a)<> [1,5;-2,9;3,3] then pause,end
a=sparse([1 2;-3 4;5 -6]+0*%i);
//if cumsum(a)<>3+0*%i then pause,end
//if cumsum(a+0*a)<>3+0*%i then pause,end




//cumprod
a=[1 2;-3 4;5 -6];
if or(cumprod(a)<>[1 -30;-3 -120;-15 720]) then pause,end
if or(cumprod(a+0)<> [1 -30;-3 -120;-15 720]) then pause,end
if or(cumprod(a,1)<>[1 2;-3 8;-15 -48]) then pause,end
if or(cumprod(a+0,1)<>[1 2;-3 8;-15 -48]) then pause,end
n=1;
if or(cumprod(a,n)<>[1 2;-3 8;-15 -48]) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 8;-15 -48]) then pause,end
if or(cumprod(a,'r')<>[1 2;-3 8;-15 -48]) then pause,end
if or(cumprod(a+0,'r')<>[1 2;-3 8;-15 -48]) then pause,end
n='r';
if or(cumprod(a,n)<>[1 2;-3 8;-15 -48]) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 8;-15 -48]) then pause,end

if or(cumprod(a,2)<>[1 2;-3 -12;5 -30]) then pause,end
if or(cumprod(a+0,2)<>[1 2;-3 -12;5 -30]) then pause,end
n=2;
if or(cumprod(a,n)<>[1 2;-3 -12;5 -30]) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 -12;5 -30]) then pause,end
if or(cumprod(a,'c')<>[1 2;-3 -12;5 -30]) then pause,end
if or(cumprod(a+0,'c')<>[1 2;-3 -12;5 -30]) then pause,end
n='c';
if or(cumprod(a,n)<>[1 2;-3 -12;5 -30]) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 -12;5 -30]) then pause,end

a=[1 2;-3 4;5 -6]+0*%i;
if cumprod(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
if cumprod(a+0)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
if or(cumprod(a,1)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
if or(cumprod(a+0,1)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
n=1;
if or(cumprod(a,n)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
if or(cumprod(a,'r')<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
if or(cumprod(a+0,'r')<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
n='r';
if or(cumprod(a,n)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 8;-15 -48]+0*%i) then pause,end

if or(cumprod(a,2)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
if or(cumprod(a+0,2)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
n=2;
if or(cumprod(a,n)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
if or(cumprod(a,'c')<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
if or(cumprod(a+0,'c')<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
n='c';
if or(cumprod(a,n)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end
if or(cumprod(a+0,n)<>[1 2;-3 -12;5 -30]+0*%i) then pause,end

a=[];
if cumprod(a)<>[] then pause,end
if cumprod([])<>[] then pause,end
if cumprod(a,1)<>[] then pause,end
if cumprod([],1)<>[] then pause,end
n=1;
if cumprod(a,n)<>[] then pause,end
if cumprod([],n)<>[] then pause,end
if cumprod(a,'r')<>[] then pause,end
if cumprod([],'r')<>[] then pause,end
n='r';
if cumprod(a,n)<>[] then pause,end
if cumprod([],n)<>[] then pause,end

if cumprod(a,2)<>[] then pause,end
if cumprod([],2)<>[] then pause,end
n=2;
if cumprod(a,n)<>[] then pause,end
if cumprod([],n)<>[] then pause,end
if cumprod(a,'c')<>[] then pause,end
if cumprod([],'c')<>[] then pause,end
n='c';
if cumprod(a,n)<>[] then pause,end
if cumprod([],n)<>[] then pause,end


a=sparse([1 2;-3 4;5 -6]);
//if cumprod(a)<> [1 -30;-3 -120;-15 720] then pause,end
//if cumprod(a+0*a)<> [1 -30;-3 -120;-15 720] then pause,end
a=sparse([1 2;-3 4;5 -6]+0*%i);
//if cumprod(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if cumprod(a+0*a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end

//maxi
a=[1 2;-3 4;5 -6];
if or(maxi(a)<>5) then pause,end
if or(maxi(a+0)<>5) then pause,end
if or(maxi(a,'r')<>[5 4]) then pause,end
if or(maxi(a+0,'r')<>[5 4]) then pause,end
n='r';
if or(maxi(a,n)<>[5 4]) then pause,end
if or(maxi(a+0,n)<>[5 4]) then pause,end

if or(maxi(a,'c')<>[2;4;5]) then pause,end
if or(maxi(a+0,'c')<>[2;4;5]) then pause,end
n='c';
if or(maxi(a,n)<>[2;4;5]) then pause,end
if or(maxi(a+0,n)<>[2;4;5]) then pause,end

//a=[1 2;-3 4;5 -6]+0*%i;
//if maxi(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if maxi(a+0)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if or(maxi(a,'r')<>[5 4]+0*%i) then pause,end
//if or(maxi(a+0,'r')<>[5 4]+0*%i) then pause,end
//n='r';
//if or(maxi(a,n)<>[5 4]+0*%i) then pause,end
//if or(maxi(a+0,n)<>[5 4]+0*%i) then pause,end

//if or(maxi(a,'c')<>[2;4;5]+0*%i) then pause,end
//if or(maxi(a+0,'c')<>[2;4;5]+0*%i) then pause,end
//n='c';
//if or(maxi(a,n)<>[2;4;5]+0*%i) then pause,end
//if or(maxi(a+0,n)<>[2;4;5]+0*%i) then pause,end

a=[];
if maxi(a)<>[] then pause,end
if maxi([])<>[] then pause,end

if maxi(a,'r')<>[] then pause,end
if maxi([],'r')<>[] then pause,end
n='r';
if maxi(a,n)<>[] then pause,end
if maxi([],n)<>[] then pause,end

if maxi(a,'c')<>[] then pause,end
if maxi([],'c')<>[] then pause,end
n='c';
if maxi(a,n)<>[] then pause,end
if maxi([],n)<>[] then pause,end


a=sparse([1 2;-3 4;5 -6]);
//if maxi(a)<>5 then pause,end
//if maxi(a+0*a)<>5 then pause,end
a=sparse([1 2;-3 4;5 -6]+0*%i);
//if maxi(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if maxi(a+0*a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end


//mini
a=[1 2;-3 4;5 -6];
if or(mini(a)<>-6) then pause,end
if or(mini(a+0)<>-6) then pause,end
if or(mini(a,'r')<>[-3 -6]) then pause,end
if or(mini(a+0,'r')<>[-3 -6]) then pause,end
n='r';
if or(mini(a,n)<>[-3 -6]) then pause,end
if or(mini(a+0,n)<>[-3 -6]) then pause,end

if or(mini(a,'c')<>[1;-3;-6]) then pause,end
if or(mini(a+0,'c')<>[1;-3;-6]) then pause,end
n='c';
if or(mini(a,n)<>[1;-3;-6]) then pause,end
if or(mini(a+0,n)<>[1;-3;-6]) then pause,end

//a=[1 2;-3 4;5 -6]+0*%i;
//if mini(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if mini(a+0)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if or(mini(a,'r')<>[-3 -6]+0*%i) then pause,end
//if or(mini(a+0,'r')<>[-3 -6]+0*%i) then pause,end
//n='r';
//if or(mini(a,n)<>[-3 -6]+0*%i) then pause,end
//if or(mini(a+0,n)<>[-3 -6]+0*%i) then pause,end

//if or(mini(a,'c')<>[1;-3;-6]+0*%i) then pause,end
//if or(mini(a+0,'c')<>[1;-3;-6]+0*%i) then pause,end
//n='c';
//if or(mini(a,n)<>[1;-3;-6]+0*%i) then pause,end
//if or(mini(a+0,n)<>[1;-3;-6]+0*%i) then pause,end

a=[];
if mini(a)<>[] then pause,end
if mini([])<>[] then pause,end

if mini(a,'r')<>[] then pause,end
if mini([],'r')<>[] then pause,end
n='r';
if mini(a,n)<>[] then pause,end
if mini([],n)<>[] then pause,end

if mini(a,'c')<>[] then pause,end
if mini([],'c')<>[] then pause,end
n='c';
if mini(a,n)<>[] then pause,end
if mini([],n)<>[] then pause,end


a=sparse([1 2;-3 4;5 -6]);
//if mini(a)<>-6 then pause,end
//if mini(a+0*a)<>-6 then pause,end
a=sparse([1 2;-3 4;5 -6]+0*%i);
//if mini(a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end
//if mini(a+0*a)<>[1 -30;-3 -120;-15 720]+0*%i then pause,end

//sort
a=[5 1 3 2 4]
if or(sort(a)<>[5 4 3 2 1]) then pause,end
if or(sort(a+0)<>[5 4 3 2 1]) then pause,end
[s,k]=sort(a);
if or(k<>[1 5 3 4 2]) then pause,end
if or(s<>[5 4 3 2 1]) then pause,end
[s,k]=sort(a+0);
if or(k<>[1 5 3 4 2]) then pause,end
if or(s<>[5 4 3 2 1]) then pause,end

a=string([5 1 3 2 4])
//if or(sort(a)<>string(1:5)) then pause,end
if or(sort(string([5 1 3 2 4]))<>string(1:5)) then pause,end
//[s,k]=sort(a);
//if or(k<>[2 4 3 5 1]) then pause,end
//if or(s<>string(1:5)) then pause,end
[s,k]=sort(string([5 1 3 2 4]));
if or(k<>[2 4 3 5 1]) then pause,end
if or(s<>string(1:5)) then pause,end

a=[]
if sort(a)<>[] then pause,end
[s,k]=sort(a);if s<>[]|k<>[] then pause,end

if sort([])<>[] then pause,end
[s,k]=sort([]);if s<>[]|k<>[] then pause,end

//kron
a=[1 2];b=[3;4];
if or(kron(a,b)<>[3 6;4 8]) then pause,end
if or(kron(a+0,b)<>[3 6;4 8]) then pause,end
if or(kron(a,b+0)<>[3 6;4 8]) then pause,end
if or(kron(a+0,b+0)<>[3 6;4 8]) then pause,end
if kron([],b)<>[] then pause,end
if kron([],b+0)<>[] then pause,end
a=[];
if kron(a,b)<>[] then pause,end
if kron(a,b+0)<>[] then pause,end
a=[1 2];b=[]
if kron(a,b)<>[] then pause,end
if kron(a+0,b)<>[] then pause,end
if kron(a,[])<>[] then pause,end
if kron(a+0,[])<>[] then pause,end
a=[];b=[];
if kron(a,b)<>[] then pause,end
if kron(a,[])<>[] then pause,end
if kron([],b)<>[] then pause,end
if kron([],[])<>[] then pause,end

//matrix
a=[1 2 3 4 5 6];
n=1;m=6;
if or(matrix(a,1,6)<>a) then pause,end
if or(matrix(a,n,6)<>a) then pause,end
if or(matrix(a,1,m)<>a) then pause,end
if or(matrix(a,n,m)<>a) then pause,end
if or(matrix(a+0,1,6)<>a) then pause,end
if or(matrix(a+0,n,6)<>a) then pause,end
if or(matrix(a+0,1,m)<>a) then pause,end
if or(matrix(a+0,n,m)<>a) then pause,end

n=3;m=2; b=[1 4;2 5;3 6];
if or(matrix(a,3,2)<>b) then pause,end
if or(matrix(a,n,2)<>b) then pause,end
if or(matrix(a,3,m)<>b) then pause,end
if or(matrix(a,n,m)<>b) then pause,end
if or(matrix(a+0,3,2)<>b) then pause,end
if or(matrix(a+0,n,2)<>b) then pause,end
if or(matrix(a+0,3,m)<>b) then pause,end
if or(matrix(a+0,n,m)<>b) then pause,end

a=[1+%i 2 3 4 5 6];
n=1;m=6;
if or(matrix(a,1,6)<>a) then pause,end
if or(matrix(a,n,6)<>a) then pause,end
if or(matrix(a,1,m)<>a) then pause,end
if or(matrix(a,n,m)<>a) then pause,end
if or(matrix(a+0,1,6)<>a) then pause,end
if or(matrix(a+0,n,6)<>a) then pause,end
if or(matrix(a+0,1,m)<>a) then pause,end
if or(matrix(a+0,n,m)<>a) then pause,end

n=3;m=2; b=[1+%i 4;2 5;3 6];
if or(matrix(a,3,2)<>b) then pause,end
if or(matrix(a,n,2)<>b) then pause,end
if or(matrix(a,3,m)<>b) then pause,end
if or(matrix(a,n,m)<>b) then pause,end
if or(matrix(a+0,3,2)<>b) then pause,end
if or(matrix(a+0,n,2)<>b) then pause,end
if or(matrix(a+0,3,m)<>b) then pause,end
if or(matrix(a+0,n,m)<>b) then pause,end

a=string([1 2 3 4 5 6]);n=1;m=6;
if or(matrix(a,1,6)<>a) then pause,end
if or(matrix(a,n,6)<>a) then pause,end
if or(matrix(a,1,m)<>a) then pause,end
if or(matrix(a,n,m)<>a) then pause,end
if or(matrix(a+a,1,6)<>a+a) then pause,end
if or(matrix(a+a,n,6)<>a+a) then pause,end
if or(matrix(a+a,1,m)<>a+a) then pause,end
if or(matrix(a+a,n,m)<>a+a) then pause,end

n=3;m=2; b=string([1 4;2 5;3 6]);
if or(matrix(a,3,2)<>b) then pause,end
if or(matrix(a,n,2)<>b) then pause,end
if or(matrix(a,3,m)<>b) then pause,end
if or(matrix(a,n,m)<>b) then pause,end
if or(matrix(a+a,3,2)<>b+b) then pause,end
if or(matrix(a+a,n,2)<>b+b) then pause,end
if or(matrix(a+a,3,m)<>b+b) then pause,end
if or(matrix(a+a,n,m)<>b+b) then pause,end

a=[1 2 3 4 5 6]+%s;
n=1;m=6;
if or(matrix(a,1,6)<>a) then pause,end
if or(matrix(a,n,6)<>a) then pause,end
if or(matrix(a,1,m)<>a) then pause,end
if or(matrix(a,n,m)<>a) then pause,end
if or(matrix(a+0,1,6)<>a) then pause,end
if or(matrix(a+0,n,6)<>a) then pause,end
if or(matrix(a+0,1,m)<>a) then pause,end
if or(matrix(a+0,n,m)<>a) then pause,end

n=3;m=2; b=[1 4;2 5;3 6]+%s;
if or(matrix(a,3,2)<>b) then pause,end
if or(matrix(a,n,2)<>b) then pause,end
if or(matrix(a,3,m)<>b) then pause,end
if or(matrix(a,n,m)<>b) then pause,end
if or(matrix(a+0,3,2)<>b) then pause,end
if or(matrix(a+0,n,2)<>b) then pause,end
if or(matrix(a+0,3,m)<>b) then pause,end
if or(matrix(a+0,n,m)<>b) then pause,end

a=[1+%i 2 3 4 5 6]+%s;
n=1;m=6;
if or(matrix(a,1,6)<>a) then pause,end
if or(matrix(a,n,6)<>a) then pause,end
if or(matrix(a,1,m)<>a) then pause,end
if or(matrix(a,n,m)<>a) then pause,end
if or(matrix(a+0,1,6)<>a) then pause,end
if or(matrix(a+0,n,6)<>a) then pause,end
if or(matrix(a+0,1,m)<>a) then pause,end
if or(matrix(a+0,n,m)<>a) then pause,end

n=3;m=2; b=[1+%i 4;2 5;3 6]+%s;
if or(matrix(a,3,2)<>b) then pause,end
if or(matrix(a,n,2)<>b) then pause,end
if or(matrix(a,3,m)<>b) then pause,end
if or(matrix(a,n,m)<>b) then pause,end
if or(matrix(a+0,3,2)<>b) then pause,end
if or(matrix(a+0,n,2)<>b) then pause,end
if or(matrix(a+0,3,m)<>b) then pause,end
if or(matrix(a+0,n,m)<>b) then pause,end

//clean
a=[1 1.d-12 1.d-5 2d8];
b=[1 0 0 2d8];
if or(clean(a)<>b) then pause,end
if or(clean(a+0)<>b) then pause,end
epsa=1.d-10;
if or(clean(a,epsa)<>b) then pause,end
if or(clean(a+0,epsa)<>b) then pause,end
if or(clean(a,epsa+0)<>b) then pause,end
if or(clean(a+0,epsa+0)<>b) then pause,end
epsr=1.d-5;b=[0 0 0 2d8];
if or(clean(a,epsa,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa+0,epsr+0)<>b) then pause,end

a=[1+%i 1.d-12 1.d-5 2d8];
b=[1+%i 0 0 2d8];
if or(clean(a)<>b) then pause,end
if or(clean(a+0)<>b) then pause,end
epsa=1.d-10;
if or(clean(a,epsa)<>b) then pause,end
if or(clean(a+0,epsa)<>b) then pause,end
if or(clean(a,epsa+0)<>b) then pause,end
if or(clean(a+0,epsa+0)<>b) then pause,end
epsr=1.d-5;b=[0+0*%i 0 0 2d8];
if or(clean(a,epsa,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa,epsr+0)<>b) then pause,end
if or(clean(a,epsa+0,epsr)<>b) then pause,end
if or(clean(a+0,epsa+0,epsr+0)<>b) then pause,end
