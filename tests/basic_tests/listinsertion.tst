// Copyright INRIA
// test des insertions dans les listes
a=1,b=3;c=2;d=[1 2 3];e=[3 4 5];f=[10;20];g=[44 55];
h=5;i=4;a0=10;c0=20;b0=30;g0=[44 55 66; 10 20 30];


l=list();l(0)=a;if l<>list(a) then pause,end
l=list();l(1)=a;if l<>list(a) then pause,end
l=list(a);l(2)=b;if l<>list(a,b) then pause,end
l=list(a);l(0)=b;if l<>list(b,a) then pause,end
l=list(a);l(1)=c;if l<>list(c)  then pause,end
l=list(a);l(1)=d;if l<>list(d)  then pause,end
l=list(d);l(1)=a;if l<>list(a) then pause,end
l=list(a);l(1)=null();if l<>list() then pause,end
l=list(a,c);l(1)=null();if l<>list(c) then pause,end
l=list(a,c);l(2)=null();if l<>list(a) then pause,end
l=list(a,c,b);l(2)=null();if l<>list(a,b) then pause,end
l=list(a,c,b);l(2)=e;if l<>list(a,e,b) then pause,end
l=list(a,e,b);l(2)=a;if l<>list(a,c,b) then pause,end

l=list();l(0)=null();if l<>list() then pause,end
l=list();l(1)=null();if l<>list() then pause,end
//
l=list(a);l(0)=list(c,b);if l<>list(list(c,b),a) then pause,end
l=list(a);l(2)=list(c,b);if l<>list(a,list(c,b)) then pause,end
l=list(a,list(c,b));l(3)=i;if l<>list(a,list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(1)=null();if l<>list(list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(2)=null();if l<>list(a,i) then pause,end
l=list(a,list(c,b),i);l(3)=null();if l<>list(a,list(c,b)) then pause,end
//
l=list(a,list(c,b,i),h);l(2)(2)=a0;if l<>list(a,list(c,a0,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=f;if l<>list(a,list(c,f,i),h) then pause,end
l=list(a,list(c,f,i),h);l(2)(2)=b;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b,i),h);l(2)(3)=a0;if l<>list(a,list(c,b,a0),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=f;if l<>list(a,list(c,b,f),h) then pause,end
l=list(a,list(c,b,f),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=a;if l<>list(a,list(a,c,b),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=null();if l<>list(a,list(c,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(1)=null();if l<>list(a,list(b,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=null();if l<>list(a,list(c,b),h) then pause,end
l=list(a,list(c,b),h);l(2)(3)=g;if l<>list(a,list(c,b,g),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=e;if l<>list(a,list(e,c,b),h) then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=0;if l<>list(a,list(c,list(0,c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=null();if l<>list(a,list(c,list(c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=g;if l<>list(a,list(c,list(g,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(0)=g;if l<>list(a,list(c,list(g,a0,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=g;if l<>list(a,list(c,list(a0,c0,g),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=null();if l<>list(a,list(c,list(a0,c0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(4)=g;if l<>list(a,list(c,list(a0,c0,b0,g),b),h); then
pause,end
l=list();l(5)=33;
ll=l;

l=list(a,1/%s);l(2)('num')=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)('num')(1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')(1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')(1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)('num')(1,1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')(1,1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end

l=list(a,1/%s,g);l(2)('num')=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')(1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1,1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1,1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end

l=list(a,1/%s,g);l(2)('num')(1,2)=33;l(2)('den')(1,2)=%s+1;
if l<>list(a,[1 33]./[%s %s+1],g) then pause,end


//a=1,b=3;c=2;d=[1 2 3];e=[3 4 5];f=[10;20];g=[44 55];
//h=5;i=4;a0=10;c0=20;b0=30;g0=[44 55 66; 10 20 30];
l=list(a);l(1)(1)=3;x=a;x(1)=3;if l<>list(x) then pause,end
l=list(e);l(1)(2)=[];x=e;x(2)=[];if l<>list(x) then pause,end
l=list(a);l(1)(1,1)=3;x=a;x(1,1)=3;if l<>list(x) then pause,end
l=list(e);l(1)(1,2)=[];x=e;x(1,2)=[];if l<>list(x) then pause,end
l=list(a);l(1)(5,5)=3;x=a;x(5,5)=3;if l<>list(x) then pause,end
l=list(g0);l(1)(2,3)=3;x=g0;x(2,3)=3;if l<>list(x) then pause,end

l=list(b,a);l(2)(1)=3;x=a;x(1)=3;if l<>list(b,x) then pause,end
l=list(b,e);l(2)(2)=[];x=e;x(2)=[];if l<>list(b,x) then pause,end
l=list(b,a);l(2)(1,1)=3;x=a;x(1,1)=3;if l<>list(b,x) then pause,end
l=list(b,e);l(2)(1,2)=[];x=e;x(1,2)=[];if l<>list(b,x) then pause,end
l=list(b,a);l(2)(5,5)=3;x=a;x(5,5)=3;if l<>list(b,x) then pause,end
l=list(b,g0);l(2)(2,3)=3;x=g0;x(2,3)=3;if l<>list(b,x) then pause,end
l=list(b,g0);l(2)(2,2:3)=[1 3];x=g0;x(2,2:3)=[1 3];if l<>list(b,x) then pause,end


a=1:10,b=3:2:33;c=(-10:0)';d=[1 2+%s 3];e=['1';'2';'3';'4'];
f=[%t %t %f];g=['12345','abcdefghijk'];
h=rand(3,3);i=eye(10,10);a0=10+%s;c0=20;b0=sparse(eye(30,30));

l=list();l(0)=a;if l<>list(a) then pause,end
l=list();l(1)=a;if l<>list(a) then pause,end
l=list(a);l(2)=b;if l<>list(a,b) then pause,end
l=list(a);l(0)=b;if l<>list(b,a) then pause,end
l=list(a);l(1)=c;if l<>list(c)  then pause,end
l=list(a);l(1)=d;if l<>list(d)  then pause,end
l=list(d);l(1)=a;if l<>list(a) then pause,end
l=list(a);l(1)=null();if l<>list() then pause,end
l=list(a,c);l(1)=null();if l<>list(c) then pause,end
l=list(a,c);l(2)=null();if l<>list(a) then pause,end
l=list(a,c,b);l(2)=null();if l<>list(a,b) then pause,end
l=list(a,c,b);l(2)=e;if l<>list(a,e,b) then pause,end
l=list(a,e,b);l(2)=a;if l<>list(a,c,b) then pause,end

l=list();l(0)=null();if l<>list() then pause,end
l=list();l(1)=null();if l<>list() then pause,end
//
l=list(a);l(0)=list(c,b);if l<>list(list(c,b),a) then pause,end
l=list(a);l(2)=list(c,b);if l<>list(a,list(c,b)) then pause,end
l=list(a,list(c,b));l(3)=i;if l<>list(a,list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(1)=null();if l<>list(list(c,b),i) then pause,end
l=list(a,list(c,b),i);l(2)=null();if l<>list(a,i) then pause,end
l=list(a,list(c,b),i);l(3)=null();if l<>list(a,list(c,b)) then pause,end
//
l=list(a,list(c,b,i),h);l(2)(2)=a0;if l<>list(a,list(c,a0,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=f;if l<>list(a,list(c,f,i),h) then pause,end
l=list(a,list(c,f,i),h);l(2)(2)=b;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b,i),h);l(2)(3)=a0;if l<>list(a,list(c,b,a0),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=f;if l<>list(a,list(c,b,f),h) then pause,end
l=list(a,list(c,b,f),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end

l=list(a,list(c,b),h);l(2)(3)=i;if l<>list(a,list(c,b,i),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=a;if l<>list(a,list(a,c,b),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(2)=null();if l<>list(a,list(c,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(1)=null();if l<>list(a,list(b,i),h) then pause,end
l=list(a,list(c,b,i),h);l(2)(3)=null();if l<>list(a,list(c,b),h) then pause,end
l=list(a,list(c,b),h);l(2)(3)=g;if l<>list(a,list(c,b,g),h) then pause,end
l=list(a,list(c,b),h);l(2)(0)=e;if l<>list(a,list(e,c,b),h) then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=0;if l<>list(a,list(c,list(0,c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=null();if l<>list(a,list(c,list(c0,b0),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(1)=g;if l<>list(a,list(c,list(g,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(0)=g;if l<>list(a,list(c,list(g,a0,c0,b0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=g;if l<>list(a,list(c,list(a0,c0,g),b),h); then pause,end
l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(3)=null();if l<>list(a,list(c,list(a0,c0),b),h); then pause,end

l=list(a,list(c,list(a0,c0,b0),b),h);
l(2)(2)(4)=g;if l<>list(a,list(c,list(a0,c0,b0,g),b),h); then pause,end

l=list(a,1/%s);l(2)('num')=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)('num')(1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')(1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')(1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end
l=list(a,1/%s);l(2)('num')(1,1)=33;if l<>list(a,33/%s) then pause,end
l=list(a,1/%s);l(2)('num')(1,1)=%s+1;if l<>list(a,((%s+1)/%s)) then pause,end
l=list(a,1/%s);l(2)('den')(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1)) then pause,end

l=list(a,1/%s,g);l(2)('num')=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')(1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1,1)=33;if l<>list(a,33/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('num')(1,1)=%s+1;if l<>list(a,(%s+1)/%s,g) then pause,end
l=list(a,1/%s,g);l(2)('den')(1,1)=%s^1+1;if l<>list(a,1/(%s^2+1),g) then pause,end

l=list(a,1/%s,g);l(2)('num')(1,2)=33;l(2)('den')(1,2)=%s+1;
if l<>list(a,[1 33]./[%s %s+1],g) then pause,end

//test with field names not defined in the structure here row1 and row2
deff('M=%to_e(varargin)',[
'M=varargin($)'
'select varargin(1)'
'case ''row2'''
'  M.N=M.N(2,:)'
'  M.V=M.V(2,:)'
'case ''row1'''
'  M.N=M.N(1,:)'
'  M.V=M.V(1,:)'
'else'
'  M.N=M.N(varargin(1:$-1))'
'  M.V=M.V(varargin(1:$-1))'
'end'])

deff('M=%to_i_to(varargin)',[
'M=varargin($);N=varargin($-1)'
'select varargin(1)'
'case ''row2'''
'  M.N(2,:)=N.N'
'  M.V(2,:)=N.V'
'case ''row1'''
'  M.N(1,:)=N.N'
'  M.V(1,:)=N.V'
'else'
'  M.N(varargin(1:$-2))=N.N'
'  M.V(varargin(1:$-2))=N.V'
'end'])
M=mlist(['to','V','N'],[1 2 3;4 5 6],['a','b','c';'d','e','f']);
M.row1(2)=M.row2(1)
l=list(1,M,2);
l(2).row1(2)=M.row1(1)
l(1)='foo'
