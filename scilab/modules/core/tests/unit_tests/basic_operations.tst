//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab basic operations

pi=%pi;
i=%i;
e=%e;
1;
a=1;
b=[1 2 3];
c=[1 2 3;4 5 6];
d=[1 2 3;4 5 6]';
d=[1 2 3 4;4 5 6 7;8 9 10 11;12 13 14 15];
d=[1 2;3 4];
e1=[[1 2] [3 4];[5 6 7 8];[9;10;11;12]'];
f([1 3 5],[1 2 3])=[-1 -2 -3;-4 -5 -6;-7 -8 -9];
g=[i,2,3;1 i 3;1 2 i];
h=[i 1 2 i 3];
o=[1 i i 2 3]';
b(2)=3;
e1(1,3)=1;
p='apcdefghijklmnopqrstuvwxyz0123456789';
// test de stackg
a,b,c,d,e1,f,g,h,o,p
assert_checkequal(b(2),3);
assert_checkequal(e1(1,3),1);
assert_checkequal(e1([1 2],[3 4]),[1 4;7 8]);
assert_checkequal(e1(1:2,:),[1 2 1 4;5 6 7 8]);
assert_checkequal(e1(:,4),[4; 8; 12]);
assert_checkequal(e1(:,:),[1 2 1 4; 5 6 7 8; 9 10 11 12]);
assert_checkequal(g(1,1),i);
assert_checkequal(g(:,1),[i;1;1]);
assert_checkequal(g(1:2,:),[i,2,3; 1,i,3]);
assert_checkequal(g(:,:),[i,2,3;1,i,3;1,2,i]);
assert_checkequal(o',[1 i i 2 3]);
//test de stack2
//
// additions...
assert_checkequal([1 2 3;4 5 6]-[1 2 3;4 5 6],[0,0,0;0,0,0]);
assert_checkequal([1 2 3;4 5 6]+2*[1 2 3;4 5 6]-[1 2 3;4 5 6]*3, [0,0,0;0,0,0]);
assert_checktrue([1 i 3;4 5 i]+2*[1 i 3;4 5 i]-[1 i 3;4 5 i]*3 == [0,0,0;0,0,0]);
assert_checktrue([2*i -4*i 2*i]+2*[i -2*i i]+i*[2 -4 2]-[i -2*i i]*2 -2*[2 -4 2]*i == [0,0,0]);
//
assert_checkequal(2\[4 8 16]/2-[1 2 4],[0,0,0]);
assert_checktrue(i\[2*i 2 i*4]/i-[-i*2 -2 -4*i] == [0,0,0]);
// element wise operations
assert_checkequal([1 2;3 4;5 6].*[1 2;3 4;5 6]-[1 4;9 16;25 36],[0,0;0,0;0,0]);
assert_checktrue([1 2;3 4;5 6*i].*[1 2;3 4;5 6]-[1 4;9 16;25 36*i] == [0,0;0,0;0,0]);
assert_checktrue([1 2;3 4;5 6].*[1 2;3 4;5 6*i]-[1 4;9 16;25 36*i] == [0,0;0,0;0,0]);
assert_checktrue([1 2;3 4;5 6*i].*[1 2;3 4;5 6*i]-[1 4;9 16;25 -36] == [0,0;0,0;0,0]);
//
assert_checkequal([2 9 8;3 10 15]./[2 3 4;3 5 5]-[1 3 2;1 2 3],[0,0,0;0,0,0]);
assert_checktrue([2 9*i 8;3 10 15]./[2 3 4;3 5 5]-[1 3*i 2;1 2 3] == [0,0,0;0,0,0]);
assert_checktrue([2 9 8;3 10 15]./[2*i 3 4;3 5 5]-[-i 3 2;1 2 3] == [0,0,0;0,0,0]);
assert_checktrue([2*i 9 8;3 10 15]./[2*i 3 4;3 5 5]-[1 3 2;1 2 3] == [0,0,0;0,0,0]);
//
assert_checkequal([2 3 4;3 5 5].\[2 9 8;3 10 15]-[1 3 2;1 2 3],[0,0,0;0,0,0]);
assert_checktrue([2 i 4;3 5 5].\[2 9 8;3 10 15]-[1 -9*i 2 ;1 2 3] == [0,0,0;0,0,0]);
assert_checktrue([2 3 4;3 5 5].\[i 9 8;3 10 15]-[.5*i 3 2;1 2 3] == [0,0,0;0,0,0]);
assert_checktrue([i 3 4;3 5 5].\[i 9 8;3 10 15]-[1 3 2;1 2 3] == [0,0,0;0,0,0]);
//multiplication
assert_checkequal([1 2 3;4 5 6]*[3;2;1]-[10;28],[0;0]);
assert_checktrue([1 i 3;4 5 6]*[3;2;1]-[6+2*i;28] == [0;0]);
assert_checktrue([1 2 3;4 5 6]*[i;2;1]-[i+7;4*i+16] == [0;0]);
assert_checktrue([1 i 3;2*i,-i,1]*[i;-i;i]-[4*i+1;-3+i] == [0;0]);
//eye
assert_checkequal(eye(4,4),[1,0,0,0;0,1,0,0;0,0,1,0;0,0,0,1]);
//a+-b*eye a*eye+-b a+-eye*b eye*a+-b
assert_checkequal(-2*eye()+[1 2;3 4]+eye()*2-[1 2;3 4],[0,0;0,0]);
assert_checktrue(-2*i*eye()+[1 2;3 4]+eye()*2*i-[1 2;3 4] == [0 0;0 0]);
// :
assert_checkequal(1:10,[1 2 3 4 5 6 7 8 9 10]);
assert_checkalmostequal(1:.1:2,[1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2]);
// for
for k=1:3,for l=1:2,a(k,l)=k+l;end;end;
assert_checkequal(a,[2,3;3,4;4,5]);
assert_checkequal(diag([1 2 3])-[1 0 0;0 2 0;0 0 3],[0 0 0;0 0 0;0 0 0]);
assert_checktrue(diag([1 i 2])-[1 0 0;0 i 0;0 0 2] == [0,0,0;0,0,0;0,0,0]);
a=[1 2 3 4;5 6 7 8];
c=a;
c(1,1)=i;
assert_checkequal(diag(a)-[1;6],[0;0]);
assert_checkequal(diag(a,1)-[2;7],[0;0]);
assert_checkequal(diag(a,-1)-[5],0);
assert_checkequal(diag(a,4),[]);
assert_checktrue(diag(c)-[i;6] == [0;0]);
assert_checktrue(diag(c,1)-[2;7] == [0;0]);
assert_checktrue(diag(c,-1)-[5] == 0);
assert_checktrue(diag(c,4) == []);
//
assert_checkequal(eye(a),[1,0,0,0;0,1,0,0]);
assert_checktrue(eye(c)==[1,0,0,0;0,1,0,0]);
assert_checktrue(eye(3,3)==[1,0,0;0,1,0;0,0,1]);
assert_checkequal(eye(2,3),[1,0,0;0,1,0]);
//
assert_checkequal(ones(a),[1,1,1,1;1,1,1,1]);
assert_checkequal(ones(c),[1,1,1,1;1,1,1,1]);
assert_checkequal(ones(3,3),[1,1,1;1,1,1;1,1,1]);
assert_checkequal(ones(3,2),[1,1;1,1;1,1]);
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
