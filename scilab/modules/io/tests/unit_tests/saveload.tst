global filename;
filename = TMPDIR+'mysave';

deff('r=Check(a,b,c,d)',[
'[lhs,rhs]=argn(),r=%t'
'global filename'
'select rhs'
'case 1 then'
'a1=a;save(filename,a1);a1=null();load(filename);'
'if or(a1<>a) then r=%f,end'
'case 2 then'
'a1=a;b1=b;save(filename,a1,b1);a1=null();b1=null();load(filename);'
'if or(a1<>a)|or(b1<>b) then r=%f,end'
'case 3 then'
'a1=a;b1=b;c1=c;'
'save(filename,a1,b1,c1);'
'a1=null();b1=null();c1=null();load(filename);'
'if or(a1<>a)|or(b1<>b)|or(c1<>c) then r=%f,end'
'case 4 then'
'a1=a;b1=b;c1=c;d1=d'
'save(filename,a1,b1,c1,d1);'
'a1=null();b1=null();c1=null();;d1=null();load(filename);'
'if or(a1<>a)|or(b1<>b)|or(c1<>c)|or(d1<>d) then r=%f,end'
'end,if ~r then pause,end'])

a=[];if ~Check(a) then pause,end
a=1;if ~Check(a) then pause,end
a=[1 2 3];if ~Check(a) then pause,end
a=[1 2 3;4 5 6];if ~Check(a) then pause,end
a=1:100000;if ~Check(a) then pause,end

a=1+%i;if ~Check(a) then pause,end
a=[1 2+%i 3];if ~Check(a) then pause,end
a=[1 2 3;4 5+%i 6];if ~Check(a) then pause,end
a=1:100000;a(35)=%i;if ~Check(a) then pause,end

a='';if ~Check(a) then pause,end
a='123';if ~Check(a) then pause,end
a=['1','abcdefgs';'' 'ABDCEFGHIJKLMN'];if ~Check(a) then pause,end
a=string(1:10000);if ~Check(a) then pause,end


a=1+%s;if ~Check(a) then pause,end
a=[1 2+%s 3];if ~Check(a) then pause,end
a=[1 2 3;4 5 6+%s];if ~Check(a) then pause,end
a=1:10000;a(4)=%s;if ~Check(a) then pause,end

a=1+%i+%s;if ~Check(a) then pause,end
a=[1 2+%i 3+%s];if ~Check(a) then pause,end
a=[1 2+%s 3;4 5+%i 6];if ~Check(a) then pause,end
a=1:100;a(35)=%i;a(1)=%s;if ~Check(a) then pause,end


a=%t;if ~Check(a) then pause,end
a=rand(100,100)>0.5;if ~Check(a) then pause,end

a=mtlb_sparse(sparse(1));if ~Check(a) then pause,end
a=mtlb_sparse(sprand(100,100,0.1));if ~Check(a) then pause,end
a=mtlb_sparse(sparse(zeros(100,100)));if ~Check(a) then pause,end




a=sparse(1)==1;if ~Check(a) then pause,end
a=sprand(100,100,0.1)<0.5;if ~Check(a) then pause,end
a=sparse(zeros(100,100))==1;if ~Check(a) then pause,end

a=1:$;if ~Check(a) then pause,end
a=1:3:$;if ~Check(a) then pause,end

a=int8(1);if ~Check(a) then pause,end
a=int8(1:9);if ~Check(a) then pause,end
a=int8(1:10000);if ~Check(a) then pause,end
a=uint8(1);if ~Check(a) then pause,end
a=uint8(1:9);if ~Check(a) then pause,end
a=uint8(1:10000);if ~Check(a) then pause,end

a=int16(1);if ~Check(a) then pause,end
a=int16(1:9);if ~Check(a) then pause,end
a=int16(1:10000);if ~Check(a) then pause,end
a=uint16(1);if ~Check(a) then pause,end
a=uint16(1:9);if ~Check(a) then pause,end
a=uint16(1:10000);if ~Check(a) then pause,end

a=int32(1);if ~Check(a) then pause,end
a=int32(1:9);if ~Check(a) then pause,end
a=int32(1:10000);if ~Check(a) then pause,end
a=uint32(1);if ~Check(a) then pause,end
a=uint32(1:9);if ~Check(a) then pause,end
a=uint32(1:10000);if ~Check(a) then pause,end


deff('x=a(n)','x=n+1','n');if ~Check(a) then pause,end
deff('x=a(n)','x=n+1');if ~Check(a) then pause,end
deff('x=a','x=n+1','n');if ~Check(a) then pause,end
deff('x=a','x=n+1');if ~Check(a) then pause,end
deff('a(n)','x=n+1','n');if ~Check(a) then pause,end
deff('a(n)','x=n+1');if ~Check(a) then pause,end
deff('a()','x=n+1','n');if ~Check(a) then pause,end
deff('a()','x=n+1');if ~Check(a) then pause,end

a=list();if ~Check(a) then pause,end
a=list(1);if ~Check(a) then pause,end
a=list(1,rand(10,10),'asdfsf');if ~Check(a) then pause,end
a=list(1,rand(10,10),'asdfsf',poly(rand(10,10),'s'));if ~Check(a) then pause,end
a=list(1);a(4)='as';if ~Check(a) then pause,end
a=list(1,rand(10,10),'asdfsf',poly(rand(1,11),'s','c'));if ~Check(a) then pause,end

a=list(list());if ~Check(a) then pause,end
a=list(list(1));if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf'));if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf',poly(rand(10,10),'s')));if ~Check(a) then pause,end
a=list(1);a(4)='as';a=list(a);if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf',poly(rand(1,11),'s','c')));if ~Check(a) then pause,end


a=list(list(),2);if ~Check(a) then pause,end
a=list(list(1),rand(3,3));if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf'),'sdfs');if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf',poly(rand(10,10),'s')),$);if ~Check(a) then pause,end
a=list(1);a(4)='as';a=list(a,[]);if ~Check(a) then pause,end
a=list(list(1,rand(10,10),'asdfsf',poly(rand(1,11),'s','c')),list());if ~Check(a) then pause,end



a=[];b=1;c=[1 2 3];d=[1 2 3;4 5 6];
if ~Check(a,b) then pause,end
if ~Check(a,b,c) then pause,end
if ~Check(a,b,c,d) then pause,end

a=1+%i;b=[1 2+%i 3];c=[1 2 3;4 5+%i 6];d=1:100000;d(35)=%i;
if ~Check(a,b) then pause,end
if ~Check(a,b,c) then pause,end
if ~Check(a,b,c,d) then pause,end

a=sprand(100,100,0.1);
b=sparse(zeros(100,100))==1;
c=list(list(1,rand(10,10),'asdfsf',poly(rand(1,11),'s','c')),list());
deff('d()','x=n+1','n');
if ~Check(a,b) then pause,end
if ~Check(a,b,c) then pause,end
if ~Check(a,b,c,d) then pause,end
